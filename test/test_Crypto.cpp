//
// Created by erxyi on 31.05.2017.
//

#define BOOST_TEST_MODULE crypto_test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "Crypto/Hash.h"

BOOST_AUTO_TEST_SUITE(hash)
    BOOST_AUTO_TEST_CASE(hash_constructors)
    {
        Hash test1("tin test #5");
        BOOST_REQUIRE_EQUAL("b45be17a415ed3cd20780f7f25991e58", test1.getString());
    }
BOOST_AUTO_TEST_SUITE_END()
