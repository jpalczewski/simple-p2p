//
// Created by erxyi on 21.05.2017.
//

#include "FileManager.h"
#include <boost/range/iterator_range.hpp>
#include <iostream>

FileManager::FileManager(const AUTHOR_LOOKUP_MAP &alm) : authorLookupMap(alm) {}

FileManager::FileManager() {}

void FileManager::setWorkingDirectory(const std::string &cwdPath) {
    using namespace boost::filesystem;
    path cwd(cwdPath);

    if(!exists(cwdPath))
        throw new std::runtime_error("FileManager::setWorkingDirectory received not-existent path!");
    if(!is_directory(cwd))
        throw new std::runtime_error("FileManager::setWorkingDirectory received path that isn't directory!");

    for(auto & file : boost::make_iterator_range(directory_iterator(cwdPath), {}))
    {
        const HASH_ARRAY &ha = MD5Utils::boostPathToHashArray(file.path());
        std::cout << file << " " << MD5Utils::hashArrayToHashASCII(ha) << std::endl;
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

RESOURCES_FIND_RESULT FileManager::findInResourcesManager(const HASH_ARRAY &hash) {
    return std::make_pair(true, std::vector<unsigned char>(1)); //TODO: to be implemented
}

AUTHORS_LIST FileManager::getAllAuthors() {
    AUTHORS_LIST al;
    for(auto kv : authorLookupMap)
    {
        al.push_back(kv.first);
    }

    return al;
}

FilePartResponse FileManager::getFilePart(const FilePartRequest &request) {
    auto file = findFileFromTable(request);

    return file.getFilePart(request);
}

FileRecord
FileManager::findFileFromTable(const GenericFileRequest &request) const
{
    auto authorFiles = authorLookupMap.find(request.authorKey);
    if(authorFiles == authorLookupMap.end())
       throw new std::runtime_error("Author not found!");

    auto file = authorFiles->second.find(request.fileHash);
    if(file == authorFiles->second.end())
        throw new std::runtime_error("File not found!");

    return file->second;
}

AUTHOR_HASH_LIST FileManager::getAllFilesFromAuthor(const AUTHOR_KEY &author_key) {
    AUTHOR_HASH_LIST author_hash_list;
    auto authorFiles = authorLookupMap.find(author_key);
    if(authorFiles==authorLookupMap.end())
        throw new std::runtime_error("Author not found!");

    for(auto & it : authorFiles->second)
    {
        author_hash_list.push_back(it.first);
    }

    return author_hash_list;
}

bool FileManager::createFile(const FileCreateRequest &request)
{
    using namespace boost::filesystem;
    auto filepath = createFilePath(request);
    std::cout << filepath.native() << " a cwd:" << cwd;
    ofstream ofs{filepath};
    if(!ofs.good())
        throw new std::runtime_error("FileManager::createFile can't create file!");
    ofs.close();
    resize_file(filepath, request.length);

    FileRecord newFile(0, filepath, request.fileHash);
    authorLookupMap[request.authorKey][request.fileHash] = newFile;

    return true;

}

boost::filesystem::path FileManager::createFilePath(const FileCreateRequest &request)
{
    return boost::filesystem::path(cwd + boost::filesystem::path::preferred_separator + request.name);
}

bool FileManager::saveFilePart(const FileSavePartRequest &request)
{
    auto file = findFileFromTable(request);
    file.saveFilePart(request);
}
