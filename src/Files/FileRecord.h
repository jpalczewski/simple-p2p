//
// Created by erxyi on 22.05.2017.
//

#ifndef SIMPLE_P2P_FILERECORD_H
#define SIMPLE_P2P_FILERECORD_H

#include "MD5Utils.h"
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
    HASH_ARRAY  md5;

public:
    FileRecord(time_t lastKnownWriteTime, const path &location, const HASH_ARRAY &md5);

    FileRecord();

    const path &getLocation() const;

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
public:
};


#endif //SIMPLE_P2P_FILERECORD_H
