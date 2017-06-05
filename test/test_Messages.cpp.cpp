//
// Created by kamil on 02/06/17.
//

#define BOOST_TEST_MODULE messages_test
#include <boost/test/unit_test.hpp>
#include <Messages/SendResourceMessage.h>
#include <Files/FileManagerTypes.h>
#include <Messages/ResourceRequestMessage.h>
#include <Messages/BroadcastMessage.h>
#include <Messages/ResourceManagementMessage.h>

BOOST_AUTO_TEST_SUITE(messagesTests)

namespace
{
    Resource getTestResource()
    {
        std::vector<unsigned char> sign(128, 0x66);
        std::vector<unsigned char> hash(16, 0x55);
        std::string name = "foo";
        int64_t size = 0x12345678;
        return Resource(name, size, hash, sign);
    }

    std::string getTestKey()
    {
        return std::string(251, (char)0xBB);
    }
}

BOOST_AUTO_TEST_CASE(sendRequestMessageConversion)
{
    Resource resource = getTestResource();
    AuthorKeyType key = getTestKey();
    SendResourceMessage message(resource, 0, 10, std::vector<unsigned char>{0x11, 0x22});
    auto bytes = message.toByteStream();
    SendResourceMessage fromBytes = SendResourceMessage::fromByteStream(bytes);
    BOOST_CHECK_EQUAL(message.getOffset(), fromBytes.getOffset());
}


BOOST_AUTO_TEST_CASE(requestResourceMessageConversion)
{
    Resource resource = getTestResource();
    AuthorKeyType key = getTestKey();
    ResourceRequestMessage message(key, resource, 44, 55);
    auto bytes = message.toByteStream();
    ResourceRequestMessage fromBytes = ResourceRequestMessage::fromByteStream(bytes);
    BOOST_CHECK_EQUAL(message.getOffset(), fromBytes.getOffset());
    BOOST_CHECK_EQUAL(key, fromBytes.getPublicKey());
    BOOST_CHECK_EQUAL(55, fromBytes.getSize());
    BOOST_CHECK_EQUAL(0x12345678, fromBytes.getResource().getSize());
    BOOST_CHECK_EQUAL("foo", fromBytes.getResource().getName());
}

BOOST_AUTO_TEST_CASE(broadcastResourceMessageConversion)
{
    Resource resource = getTestResource();
    AuthorKeyType key = getTestKey();
    std::unordered_map<std::string, std::vector<Resource>> map{{key, std::vector<Resource>{resource}}};
    BroadcastMessage message(map);
    auto bytes = message.toByteStream();
    BroadcastMessage fromBytes = BroadcastMessage::fromByteStream(bytes);
    const auto resourceMap = fromBytes.getResources();
    Resource receivedResource = resourceMap.find(key)->second[0];
    BOOST_CHECK_EQUAL(0x12345678, receivedResource.getSize());
    BOOST_CHECK_EQUAL("foo", receivedResource.getName());
}

BOOST_AUTO_TEST_CASE(resourceManagementMessageConversion)
{
    Resource resource = getTestResource();
    AuthorKeyType key = getTestKey();
    std::vector<unsigned char> sign(128, 0x11);
    ResourceManagementMessage message(key, resource, sign);
    auto bytes = message.toByteStream(true);
    ResourceManagementMessage fromBytes = ResourceManagementMessage::fromByteStream(bytes, 0);
    BOOST_CHECK_EQUAL(key, fromBytes.getPublicKey());
    BOOST_CHECK_EQUAL("foo", fromBytes.getResource().getName());
//    BOOST_CHECK_EQUAL(sign, fromBytes.getSign());
    BOOST_CHECK_EQUAL(0x12345678, fromBytes.getResource().getSize());
}

BOOST_AUTO_TEST_SUITE_END()