//
// Created by erxyi on 03.06.17.
//

#define BOOST_TEST_MODULE resourcemanager_test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include "Resources/ResourceManager.h"
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(resourceMangaer_tests)
    BOOST_AUTO_TEST_CASE(setting_resource_info_state)
    {
        std::string publicKey = "It should be a public key";
        Resource resource("test res",
                          20,
                          std::vector<unsigned char>(),
                          std::vector<unsigned char>()
        );

        resourceManager.addOwnedResource(publicKey, resource);
        auto firstResult = resourceManager.getOwnedResourceInfo(publicKey, resource);
        bool compareResult = Resource::State::Active ==  firstResult.getResourceState();
        BOOST_REQUIRE_EQUAL(true, compareResult);
        resourceManager.setOwnedResourceInfoState(publicKey, resource, Resource::State::Blocked);
        auto secondResult = resourceManager.getOwnedResourceInfo(publicKey, resource);
        compareResult = Resource::State::Blocked == secondResult.getResourceState();
        BOOST_REQUIRE_EQUAL(true, compareResult);
    }
BOOST_AUTO_TEST_SUITE_END()
