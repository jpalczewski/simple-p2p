//
// Created by erxyi on 22.05.2017.
//

#include "FileRecord.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fstream>
#include <fcntl.h>
bool FileRecord::isValid() {
        std::time_t lastTime = boost::filesystem::last_write_time(location);
        if(lastTime == lastKnownWriteTime)
            return true;
        else if(getHashFromCWD() == md5) {
            lastKnownWriteTime = lastTime;
            return true;
        }
        else
            return false;
}

FileRecord::FileRecord(time_t lastKnownWriteTime, const path &location, const Hash &md5) : lastKnownWriteTime(
        lastKnownWriteTime), location(location), md5(md5) {}

const path &FileRecord::getLocation() const {
    return location;
}

Hash FileRecord::getHashFromCWD() {
    return Hash(location);
}

FilePartResponse FileRecord::getFilePart(const FilePartRequest &request) {
    using namespace boost::filesystem;
    FilePartResponse fpr;
    fpr.received.resize(request.size);

    if(!isValid() && !request.allowInvalid)
        throw new std::runtime_error("FileRecord::getFilePart error - file on disk is corrupted!");

    std::ifstream ifs = ifstream{location, std::ios::binary};

    ifs.seekg(request.offset);
    if(ifs.tellg() != request.offset)
        throw new std::runtime_error("File is too short!");

    ifs.read(reinterpret_cast<char *>(&(fpr.received[0])), request.size);
    if(ifs.gcount() < request.size);
        fpr.received.resize(ifs.gcount());
    return fpr;

}

bool FileRecord::saveFilePart(const FileSavePartRequest &request)
{
    using namespace boost::filesystem;
    std::fstream ofs = fstream{location, std::ios::binary | std::ios_base::out | std::ios_base::in};
    ofs.seekp(request.offset);
    if(ofs.tellp()!=request.offset)
        throw new std::runtime_error("Can't write to file!");
    ofs.write(reinterpret_cast<const char*>(&request.bytes[0]), request.bytes.size());
    if(ofs.fail())
        throw new std::runtime_error("FR:saveFilePart - failbit is set!");
    if(ofs.bad())
        throw new std::runtime_error("FR:saveFilePart - badbit is set!");
    ofs.flush();
    return true;
}

void FileRecord::create() {
    using namespace boost::filesystem;
    if(exists(location))
        throw new std::runtime_error("FileRecord::create error - file exists!");
    ofstream ofs{location};
    ofs.close();

}

