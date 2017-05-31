//
// Created by erxyi on 22.05.2017.
//

#ifndef SIMPLE_P2P_FILERECORD_H
#define SIMPLE_P2P_FILERECORD_H

#include "Files/Requests/FilePartResponse.h"
#include "Files/Requests/FilePartRequest.h"
#include "MD5Utils.h"
#include "Files/Requests/FileSavePartRequest.h"
#include <boost/filesystem.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/split_member.hpp>
using namespace boost::filesystem;

class FileRecord {
    friend class boost::serialization::access;

    // TODO: Reference to Resource?

    std::time_t lastKnownWriteTime;
    path        location;
    HashArray  md5;

public:
    FileRecord(time_t lastKnownWriteTime, const path &location, const HashArray &md5);
    FileRecord();
    const path &getLocation() const;
    FilePartResponse    getFilePart(const FilePartRequest& request);
    bool                saveFilePart(const FileSavePartRequest& request);
    void                create();
    void                allocate(std::size_t size);

private:
    bool isValid(); // last_write_time from boost::filesystem

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        std::string stringLocation = location.generic_string();
        ar & BOOST_SERIALIZATION_NVP(lastKnownWriteTime);
        ar & BOOST_SERIALIZATION_NVP(md5);
        ar & BOOST_SERIALIZATION_NVP(stringLocation);

    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        std::string stringLocation;
        ar & BOOST_SERIALIZATION_NVP(lastKnownWriteTime);
        ar & BOOST_SERIALIZATION_NVP(md5);
        ar & BOOST_SERIALIZATION_NVP(stringLocation);
        location = boost::filesystem::path(stringLocation);

    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    HashArray getHashFromCWD();
};


#endif //SIMPLE_P2P_FILERECORD_H
