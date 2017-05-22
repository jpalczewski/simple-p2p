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


class FileManager {

    friend class boost::serialization::access;
public:
    FileManager(const AUTHOR_LOOKUP_MAP &alm);
    FileManager();

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(authorLookupMap);
    }

    void setWorkingDirectory(const std::string & path);

    AUTHORS_LIST getAllAuthors();
    AUTHOR_HASH_LIST getAllFilesFromAuthor(const AUTHOR_KEY & author_key);
    FilePartResponse getFilePart(const FilePartRequest &request);
private:
    AUTHOR_LOOKUP_MAP authorLookupMap;

    RESOURCES_FIND_RESULT findInResourcesManager(const HASH_ARRAY &hash);


};


#endif //SIMPLE_P2P_FILEMANAGER_H
