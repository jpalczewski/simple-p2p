//
// Created by erxyi on 31.05.2017.
//

#define BOOST_TEST_MODULE resourcefile_test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "Crypto/Hash.h"
#include "Resources/ResourceManager.h"
#include "Files/FileManager.h"

BOOST_AUTO_TEST_SUITE(resourceFile)
    BOOST_AUTO_TEST_CASE(does_it_even_compile)
    {
        using namespace boost::filesystem;

        path publicKey = boost::filesystem::unique_path(),
                privateKey = boost::filesystem::unique_path();

        AuthorKey authorKey(publicKey.native(), privateKey.native());

        authorKey.generateKey(4096);

        ResourceManager rm;
        FileManager fm;

        AddFileRequest addFileRequest(authorKey.getPublicKey(), authorKey.getPrivateKey(), publicKey.native());
        auto result = fm.addFile(addFileRequest);
        Resource resource(publicKey.native(), publicKey.size(), result.first.getVector(), result.second);
    }
BOOST_AUTO_TEST_SUITE_END()