//
// Created by erxyi on 22.05.2017.
//

#define BOOST_TEST_MODULE filemanager_test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <Files/FileRecord.h>
#include "Files/MD5Utils.h"

BOOST_AUTO_TEST_SUITE(md5utils)

    BOOST_AUTO_TEST_CASE(textToHashArray)
    {
        std::string test_string = "TIN"; // 389a0cd2a94db6e335d411c94db878c7
        std::string test_hash = "389a0cd2a94db6e335d411c94db878c7";
        auto h = MD5Utils::textToHashArray(test_string);
        auto result = MD5Utils::hashArrayToHashASCII(h);
        //BOOST_TEST(result == test_hash);
        BOOST_REQUIRE_EQUAL(result, test_hash);
    }

    BOOST_AUTO_TEST_CASE(ifsToHashArray)
    {
        std::string test_string = "TIN";

        const boost::filesystem::path path = boost::filesystem::unique_path();
        const std::string tempfile = path.native();
        std::string test_hash = "389a0cd2a94db6e335d411c94db878c7";

        std::ofstream ofstream(tempfile, std::ios::binary);
        ofstream << test_string;
        ofstream.close();

        std::ifstream ifstream(tempfile, std::ios::binary);
        auto h = MD5Utils::ifstreamToHashArray(ifstream);
        auto result = MD5Utils::hashArrayToHashASCII(h);
        //BOOST_TEST(result == test_hash);
        BOOST_REQUIRE_EQUAL(result, test_hash);
        boost::filesystem::remove(path);
    }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(filerecord)
    BOOST_AUTO_TEST_CASE(serialization)
    {
        boost::filesystem::path path = boost::filesystem::unique_path();
        FileRecord fr(0, boost::filesystem::unique_path().native(), {0});
        FileRecord fri;
        std::ofstream of(path.native());
        boost::archive::xml_oarchive oa(of);
        oa << BOOST_SERIALIZATION_NVP(fr);
        of.close();

        std::ifstream ifs(path.native());
        boost::archive::xml_iarchive ia(ifs);
        ia >> BOOST_SERIALIZATION_NVP(fri);
        ifs.close();

        BOOST_REQUIRE_EQUAL(fr.getLocation().native(), fri.getLocation().native());

        boost::filesystem::remove(path);
    }
BOOST_AUTO_TEST_SUITE_END()
