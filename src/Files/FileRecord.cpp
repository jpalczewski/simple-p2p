//
// Created by erxyi on 22.05.2017.
//

#include "FileRecord.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fstream>

bool FileRecord::isValid() {
        return false;
}

FileRecord::FileRecord(time_t lastKnownWriteTime, const path &location, const HASH_ARRAY &md5) : lastKnownWriteTime(
        lastKnownWriteTime), location(location), md5(md5) {}

FileRecord::FileRecord() {}

const path &FileRecord::getLocation() const {
    return location;
}

HASH_ARRAY FileRecord::getHashFromCWD() {
    return MD5Utils::boostPathToHashArray(location);
}

FilePartResponse FileRecord::getFilePart(const FilePartRequest &request) {
    using namespace boost::filesystem;
    FilePartResponse fpr;
    fpr.received.resize(request.size);

    std::ifstream ifs = ifstream{location, std::ios::binary};

    ifs.seekg(request.offset);
    if(ifs.tellg() != request.offset)
        throw new std::runtime_error("File is too short!");

    ifs.read(&(fpr.received[0]), request.size);
    fpr.received.resize(ifs.gcount());
    return fpr;

}

bool FileRecord::saveFilePart(const FileSavePartRequest &request)
{
    using namespace boost::filesystem;
    std::ofstream ofs = ofstream{location, std::ios::binary};
    ofs.seekp(request.offset);
    ofs.write(&request.bytes[0], request.bytes.size());
    if(ofs)
        return true;
    else
        throw new std::runtime_error("Something gone wrong in FileRecord::saveFilePart");
}

