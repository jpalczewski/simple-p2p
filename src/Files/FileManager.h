//
// Created by erxyi on 21.05.2017.
//

#ifndef SIMPLE_P2P_FILEMANAGER_H
#define SIMPLE_P2P_FILEMANAGER_H

#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/list.hpp>
#include <unordered_map>
#include "FileRecord.h"
#include "AuthorKeyHasher.h"
#include "FileManagerTypes.h"
#include "FilePartRequest.h"
#include "FilePartResponse.h"
#include "FileCreateRequest.h"
#include "FileSavePartRequest.h"

class FileManager {

    friend class boost::serialization::access;
public:
    FileManager(const AuthorLookupMap &alm);
    FileManager();

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(authorLookupMap);
    }

    void setWorkingDirectory(const std::string & path);

    AuthorsList        getAllAuthors();
    AuthorFilesHashList    getAllFilesFromAuthor(const AuthorKey & author_key);

    FilePartResponse    getFilePart(const FilePartRequest &request);
    bool                saveFilePart(const FileSavePartRequest &request);
    bool                createFile(const FileCreateRequest & request);
private:
    AuthorLookupMap   authorLookupMap;
    std::string         cwd;

    ResourcesFindResult findInResourcesManager(const HASH_ARRAY &hash);

    boost::filesystem::path createFilePath(const FileCreateRequest & request);

    FileRecord findFileFromTable(const GenericFileRequest &request) const;
};


#endif //SIMPLE_P2P_FILEMANAGER_H
