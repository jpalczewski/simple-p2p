//
// Created by erxyi on 22.05.2017.
//

#ifndef SIMPLE_P2P_FILERECORD_H
#define SIMPLE_P2P_FILERECORD_H

#include "MD5Utils.h"
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

class FileRecord {

    // TODO: Reference to Resource?

    std::time_t lastKnownWriteTime;
    path        location;
    HASH_ARRAY  md5;

    bool isValid(); // last_write_time from boost::filesystem
};


#endif //SIMPLE_P2P_FILERECORD_H
