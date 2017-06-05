//
// Created by erxyi on 31.05.2017.
//

#define BOOST_TEST_MODULE crypto_test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "Crypto/Hash.h"
#include "Crypto/AuthorKey.h"

BOOST_AUTO_TEST_SUITE(hash)
    BOOST_AUTO_TEST_CASE(hash_constructors)
    {
        const std::string string = "tin test #5";
        Hash test1(string);
        BOOST_REQUIRE_EQUAL("b45be17a415ed3cd20780f7f25991e58", test1.getString());
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(authorkey_tests)
    BOOST_AUTO_TEST_CASE(does_it_works)
    {
        using namespace boost::filesystem;

        path publicKey = boost::filesystem::unique_path(),
                privateKey = boost::filesystem::unique_path();

        AuthorKey authorKey(publicKey.native(), privateKey.native());
        const std::string string = "tin test #6";
        const std::string malformed = "tin not test really #6";
        authorKey.generateKey(4096);
        auto result = authorKey.signMessage(string);

        BOOST_REQUIRE_EQUAL(true, authorKey.verifySignature(string, &result[0], result.size()));
        BOOST_REQUIRE_EQUAL(false, authorKey.verifySignature(malformed, &result[0], result.size()));
        result[5] = ~result[5];
        BOOST_REQUIRE_EQUAL(false, authorKey.verifySignature(string, &result[0], result.size()));


        remove(publicKey);
        remove(privateKey);
    }
    BOOST_AUTO_TEST_CASE(does_hash_works)
    {
        using namespace boost::filesystem;

        path publicKey = boost::filesystem::unique_path(),
                privateKey = boost::filesystem::unique_path();

        AuthorKey authorKey(publicKey.native(), privateKey.native());

        authorKey.generateKey(4096);
        auto result = authorKey.getPublicPEMHash();
        auto final_result = result.getString();

        std::cout << "A crypto ma: " << sizeof(RSA) << std::endl;

        BOOST_REQUIRE_EQUAL(final_result.size(), 32); //TODO: it can be tested in better way

        remove(publicKey);
        remove(privateKey);

    }
    BOOST_AUTO_TEST_CASE(does_vector_hash_works)
    {
        using namespace boost::filesystem;

        path publicKey = boost::filesystem::unique_path(),
                privateKey = boost::filesystem::unique_path();

        AuthorKey authorKey(publicKey.native(), privateKey.native());
        AuthorKey secondAuthor;
        authorKey.generateKey(4096);

        std::vector<unsigned char> inputVector = {2};
        std::string testVector(inputVector.begin(), inputVector.end()); //lol
        auto signResult = authorKey.signMessage(inputVector);
        std::cout << "Sign size:" << signResult.size() << std::endl;

        auto verifyResult = authorKey.verifySignature(testVector, &signResult[0], signResult.size());

        BOOST_REQUIRE_EQUAL(true, verifyResult);

        //secondAuthor.

        remove(publicKey);
        remove(privateKey);
    }
BOOST_AUTO_TEST_SUITE_END()
