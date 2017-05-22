//
// Created by erxyi on 21.05.2017.
//

#ifndef SIMPLE_P2P_FILEMANAGER_H
#define SIMPLE_P2P_FILEMANAGER_H

#include <vector>
#include <unordered_map>
#include "FileRecord.h"

typedef     std::vector<unsigned char> AUTHOR_KEY; //128b
typedef     std::list<FileRecord>   AUTHOR_FILES;
typedef     std::unordered_map<AUTHOR_KEY, AUTHOR_FILES> AUTHOR_LOOKUP_MAP;
class FileManager {

};


#endif //SIMPLE_P2P_FILEMANAGER_H
