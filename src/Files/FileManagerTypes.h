//
// Created by erxyi on 22.05.2017.
//

#ifndef SIMPLE_P2P_FILEMANAGERTYPES_H
#define SIMPLE_P2P_FILEMANAGERTYPES_H

#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/list.hpp>
#include "AuthorKeyHasher.h"
#include "MD5Utils.h"

class FileRecord;

typedef     std::vector<unsigned char>                                                  AuthorKey;
typedef     std::unordered_map<HashArray, FileRecord, AuthorKeyHasher<HashArray> >    AuthorFiles;
typedef     std::unordered_map<AuthorKey, AuthorFiles, AuthorKeyHasher<AuthorKey> >     AuthorLookupMap;
typedef     std::pair<bool, AuthorKey>                                                  ResourcesFindResult;
typedef     std::vector<AuthorKey>                                                      AuthorsList;
typedef     std::vector<HashArray>                                                     AuthorFilesHashList;


#endif //SIMPLE_P2P_FILEMANAGERTYPES_H
