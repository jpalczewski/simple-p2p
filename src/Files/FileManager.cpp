//
// Created by erxyi on 21.05.2017.
//

#include "FileManager.h"
#include <boost/range/iterator_range.hpp>

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
    auto authorFiles = authorLookupMap.find(request.authorKey);
    if(authorFiles==authorLookupMap.end())
        throw new std::runtime_error("Author not found!");

    auto file = authorFiles->second.find(request.fileHash);
    if(file == authorFiles->second.end())
        throw new std::runtime_error("File not found!");

    return file->second.getFilePart(request);
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
