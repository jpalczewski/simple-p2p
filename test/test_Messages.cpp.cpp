//
// Created by kamil on 02/06/17.
//

#define BOOST_TEST_MODULE messages_test
#include <boost/test/unit_test.hpp>
#include <Messages/SendResourceMessage.h>
#include <Files/FileManagerTypes.h>
#include <Messages/ResourceRequestMessage.h>

BOOST_AUTO_TEST_SUITE(messagesTests)

namespace
{
    Resource getTestResource()
    {
        std::vector<unsigned char> sign(128, 0x66);
        std::vector<unsigned char> hash(16, 0x55);
        std::string name = "foo";
        size_t size = 1234;
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
}


BOOST_AUTO_TEST_SUITE_END()