//
// Created by erxyi on 22.05.2017.
//

#include "FileRecord.h"

bool FileRecord::isValid() {
        return false;
}

FileRecord::FileRecord(time_t lastKnownWriteTime, const path &location, const HASH_ARRAY &md5) : lastKnownWriteTime(
        lastKnownWriteTime), location(location), md5(md5) {}

FileRecord::FileRecord() {}

const path &FileRecord::getLocation() const {
    return location;
}
