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

typedef     std::vector<unsigned char> AUTHOR_KEY;
typedef     std::unordered_map<HASH_ARRAY, FileRecord, AuthorKeyHasher<HASH_ARRAY> >   AUTHOR_FILES;
typedef     std::unordered_map<AUTHOR_KEY, AUTHOR_FILES, AuthorKeyHasher<AUTHOR_KEY> > AUTHOR_LOOKUP_MAP;
typedef     std::pair<bool, AUTHOR_KEY> RESOURCES_FIND_RESULT;
typedef     std::vector<AUTHOR_KEY>     AUTHORS_LIST;
typedef     std::vector<HASH_ARRAY>     AUTHOR_HASH_LIST;


#endif //SIMPLE_P2P_FILEMANAGERTYPES_H
