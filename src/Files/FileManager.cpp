//
// Created by erxyi on 21.05.2017.
//

#include "FileManager.h"
#include <boost/range/iterator_range.hpp>
#include <iostream>

FileManager::FileManager(const AuthorLookupMap &alm) : authorLookupMap(alm) {}

FileManager::FileManager() {}

void FileManager::setWorkingDirectory(const std::string &cwdPath) {
    using namespace boost::filesystem;
    path cwd(cwdPath);

    if(!exists(cwdPath))
        throw std::runtime_error("FileManager::setWorkingDirectory received not-existent path!");
    if(!is_directory(cwd))
        throw std::runtime_error("FileManager::setWorkingDirectory received path that isn't directory!");

    for(auto & file : boost::make_iterator_range(directory_iterator(cwdPath), {}))
    {
        //const HashArray &ha = MD5Utils::boostPathToHashArray(file.path());
        const Hash ha(file.path());
        std::cout << file << " " << ha << std::endl;
        auto result = findInResourcesManager(ha);
        if(result.first)
        {
            std::time_t lastTime = last_write_time(file.path()); // TODO: it's wrong as hell
            FileRecord fr(lastTime, file.path(), ha);
            authorLookupMap[result.second][ha] = fr;
        }
    }

    this->cwd = cwdPath;
}

ResourcesFindResult FileManager::findInResourcesManager(const Hash &hash) {
    return std::make_pair(true, std::string()); //TODO: to be implemented
}

AuthorsList FileManager::getAllAuthors() {
    std::shared_lock<std::shared_timed_mutex> lock(almMutex);

    AuthorsList al;
    for(auto kv : authorLookupMap)
    {
        al.push_back(kv.first);
    }

    return al;
}

FilePartResponse FileManager::getFilePart(const FilePartRequest &request) {
    std::shared_lock<std::shared_timed_mutex> lock(almMutex);
    auto file = findFileFromTable(request);

    return file.getFilePart(request);
}

FileRecord
FileManager::findFileFromTable(const GenericFileRequest &request)
{
    std::shared_lock<std::shared_timed_mutex> lock(almMutex);
    auto authorFiles = authorLookupMap.find(request.authorKey);
    if(authorFiles == authorLookupMap.end())
       throw std::runtime_error("Author not found!");

    auto file = authorFiles->second.find(request.fileHash);
    if(file == authorFiles->second.end())
        throw std::runtime_error("File not found!");

    return file->second;
}

AuthorFilesHashList FileManager::getAllFilesFromAuthor(const AuthorKeyType &author_key) {
    std::shared_lock<std::shared_timed_mutex> lock(almMutex);

    AuthorFilesHashList author_hash_list;
    auto authorFiles = authorLookupMap.find(author_key);
    if(authorFiles==authorLookupMap.end())
        throw std::runtime_error("Author not found!");

    for(auto & it : authorFiles->second)
    {
        author_hash_list.push_back(it.first);
    }

    return author_hash_list;
}

bool FileManager::createFile(const FileCreateRequest &request)
{
    std::lock_guard<std::shared_timed_mutex> lock(almMutex);

    using namespace boost::filesystem;
    AuthorKey authorKey;
    authorKey.loadPublicKeyFromString(request.authorKey);
    auto filepath = createFilePath(request, authorKey);
    ofstream ofs{filepath};
    if(!ofs.good())
        throw std::runtime_error("FileManager::createFile can't create file!");
    ofs.close();
    resize_file(filepath, request.length);

    FileRecord newFile(0, filepath, request.fileHash);
    authorLookupMap[request.authorKey][request.fileHash] = newFile;

    return true;

}

boost::filesystem::path FileManager::createFilePath(const FileCreateRequest &request, AuthorKey authorKey)
{
    return boost::filesystem::path(
            cwd +
            boost::filesystem::path::preferred_separator +
            authorKey.getPublicPEMHash().getString() +
            boost::filesystem::path::preferred_separator +
            request.name);
}

bool FileManager::saveFilePart(const FileSavePartRequest &request)
{
    std::lock_guard<std::shared_timed_mutex> lock(almMutex);

    auto file = findFileFromTable(request);
    file.saveFilePart(request);
}

std::pair<Hash, std::vector<unsigned char> > FileManager::addFile(const AddFileRequest &request) {
    using namespace boost::filesystem;
    std::lock_guard<std::shared_timed_mutex> lock(almMutex);

    path suggestedFile(request.path);
    if(!exists(suggestedFile))
        throw std::runtime_error("FileManager::addFile - file doesn't exist!");

    if(!is_regular(suggestedFile))
        throw std::runtime_error("FileManager::addFile - file isn't a regular file!");


    AuthorKey ak;
    ak.loadPrivateKeyFromString(request.privateKey);
    std::vector<unsigned char> signResult = ak.signMessage(request.fileHash.getString());

    FileRecord fr = FileRecord(0, suggestedFile, request.fileHash);
    authorLookupMap[request.authorKey][request.fileHash] = fr;

    return std::make_pair(request.fileHash, signResult);
}

FileManager fileManagerInstance;