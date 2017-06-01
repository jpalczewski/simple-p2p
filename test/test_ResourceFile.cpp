//
// Created by erxyi on 31.05.2017.
//

#define BOOST_TEST_MODULE resourcefile_test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "Crypto/Hash.h"

BOOST_AUTO_TEST_SUITE(resource)
    BOOST_AUTO_TEST_CASE(hash_constructors)
    {
        std::string input = "tin test #5";
        Hash test1(input);
        BOOST_REQUIRE_EQUAL("b45be17a415ed3cd20780f7f25991e58", test1.getString());
    }
BOOST_AUTO_TEST_SUITE_END()