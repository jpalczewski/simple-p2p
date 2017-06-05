//
// Created by erxyi on 22.05.2017.
//

#ifndef SIMPLE_P2P_FILERECORD_H
#define SIMPLE_P2P_FILERECORD_H

#include "../Crypto/Hash.h"
#include "Requests/FilePartResponse.h"
#include "Requests/FilePartRequest.h"
#include "MD5Utils.h"
#include "Requests/FileSavePartRequest.h"
#include <boost/filesystem.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/split_member.hpp>
using namespace boost::filesystem;

class FileRecord {
public:
private:
    friend class boost::serialization::access;

    // TODO: Reference to Resource?

    std::time_t lastKnownWriteTime;
    path        location;
    Hash        md5;
    FileRecordState       state;

public:
    FileRecord(time_t lastKnownWriteTime, const path &location, const Hash &md5);
    FileRecord() : md5("", Hash::InputTextType::Invalid) {};
    const path &getLocation() const;
    FilePartResponse    getFilePart(const FilePartRequest& request);
    bool                saveFilePart(const FileSavePartRequest& request);
    void                create();

    FileRecordState getState() const;
    void setState(FileRecordState state);

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

    Hash getHashFromCWD();
};


#endif //SIMPLE_P2P_FILERECORD_H
