//
// Created by erxyi on 22.05.2017.
//

#define BOOST_TEST_MODULE filemanager_test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <Files/FileRecord.h>
#include <Files/FileManager.h>
#include "Files/MD5Utils.h"
using namespace boost::filesystem;

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
        FileRecord fr(0, boost::filesystem::unique_path().native(), Hash("", Hash::InputTextType::Invalid));
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
    BOOST_AUTO_TEST_CASE(reading)
    {
        std::string test_string = "TIN TEST #1";
        std::vector<char> test_vec(test_string.begin(), test_string.end());
        boost::filesystem::path path = boost::filesystem::unique_path();
        ofstream ofs{path};
        ofs << test_string;
        ofs.close();
        FileRecord fileRecord(0, path, Hash("", Hash::InputTextType::Invalid));

        FilePartRequest fpr({0}, {0}, 0, 1, true);
        auto response = fileRecord.getFilePart(fpr);
        BOOST_REQUIRE_EQUAL(response.received.size(), 1);
        BOOST_REQUIRE_EQUAL(response.received[0], test_string[0]);

        fpr.offset = 3;
        response = fileRecord.getFilePart(fpr);
        BOOST_REQUIRE_EQUAL(response.received[0], test_string[3]);

        fpr.offset = 0;
        fpr.size = 500;
        response = fileRecord.getFilePart(fpr);
        BOOST_REQUIRE_EQUAL_COLLECTIONS(response.received.begin(), response.received.end(), test_vec.begin(), test_vec.end());

        remove(path);
    }
    BOOST_AUTO_TEST_CASE(write)
    {
        std::string test_string = "TIN TEST #1";
        std::vector<char> test_vec(test_string.begin(), test_string.end());
        boost::filesystem::path path = boost::filesystem::unique_path();
        FileRecord fileRecord(0, path, {0});

        fileRecord.create();
        //fileRecord.allocate(test_string.size()+1);

        FileSavePartRequest fileSavePartRequest({0}, {0}, 1, 1);
        FilePartRequest filePartRequest({0}, {0}, 0, test_string.size(), true);
        for(int j = test_string.size()-1; j >= 0; --j)
        {
            fileSavePartRequest.offset = j;
            fileSavePartRequest.bytes[0] = test_string[j];
            fileRecord.saveFilePart(fileSavePartRequest);
        }
        auto response = fileRecord.getFilePart(filePartRequest);
        BOOST_REQUIRE_EQUAL_COLLECTIONS(response.received.begin(), response.received.end(), test_vec.begin(), test_vec.end());
    }
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(fileManager)
    BOOST_AUTO_TEST_CASE(addFileFromDisk)
    {
        std::string test_string = "TIN TEST #2";
        std::vector<char> test_vec(test_string.begin(), test_string.end());
        boost::filesystem::path path = boost::filesystem::unique_path();
        FileRecord fileRecord(0, path, {0});

        fileRecord.create();

        ofstream ofs(path);
        ofs << test_string;
        ofs.close();

        FileManager fm;
        AddFileRequest afr({0}, path.native());
        auto result = fm.addFile(afr);

        FilePartRequest fpr({0}, result.second, 0, test_string.size());
        auto response = fm.getFilePart(fpr);
        BOOST_REQUIRE_EQUAL_COLLECTIONS(response.received.begin(), response.received.end(), test_vec.begin(), test_vec.end());

        remove(path);

    }
BOOST_AUTO_TEST_SUITE_END()
