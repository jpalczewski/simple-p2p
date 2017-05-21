//
// Created by kamil on 13/05/17.
//

#include <iostream>
#include <thread>
#include <cassert>
#include <unordered_map>

#include "CryptoUtils.h"
#include "Messages/BroadcastMessage.h"
#include "Resources/Resource.h"

// example of calculating md5 hash of file
// create file 'helloWorld.txt' and call this function to see result
void hashExample()
{
    const std::string filePath = "helloWorld.txt";
    std::cout << "Hash: " << toMd5String(hashMd5FromFile(filePath)) << std::endl;
}

// example of signing and verifying sign with generated keys
void signExample()
{
    const std::string publicKeyFile = "rsa_public.pem";
    const std::string privateKeyFile = "rsa_private.pem";
    generateKey(publicKeyFile, privateKeyFile);

    std::string message = "Hello, world!";
    auto sign = signMessage(privateKeyFile, message);
    const std::string &publicKey = readKey(publicKeyFile);
    bool verified = verifySignature(publicKey, message, &sign[0], sign.size());

    std::cout << "Verified: " << (verified ? "yes" : "no") << std::endl;
}

// copy to cli.cpp and call to test message conversion to byte array
// TODO move to boost tests
/*
void testMessageConversion()
{
    std::string publicKey = "-----BEGIN RSA PUBLIC KEY-----\n"
            "MIGJAoGBANLVDLEKgzjGBqiNPTqRqPQynEbbNZbY2T2K+Fn3YMX8hbZ8QLn48CZI\n"
            "n4mKDQSVND1qxBCeRBxYeaavwyZ8LCqVYPCXZwkRH0QlyReQOCxyw7duEIl/F7tc\n"
            "N3/34qht2DK8lOp9IQS7sY9L5Oy73JW4jQUMcPUUg52bq7pzccWFAgMBAAE=\n"
            "-----END RSA PUBLIC KEY-----\n";
    std::string publicKey2 = "-----BEGIN RSA PUBLIC KEY-----\n"
            "NIGJAoGBANLVDLEKgzjGBqiNPTqRqPQynEbbNZbY2T2K+Fn3YMX8hbZ8QLn48CZI\n"
            "n4mKDQSVND1qxBCeRBxYeaavwyZ8LCqVYPCXZwkRH0QlyReQOCxyw7duEIl/F7tc\n"
            "N3/34qht2DK8lOp9IQS7sY9L5Oy73JW4jQUMcPUUg52bq7pzccWFAgMBAAE=\n"
            "-----END RSA PUBLIC KEY-----\n";
    assert(publicKey.length() == 251);
    int size = 55;
    const char *testHash = "0123456789012345";
    unsigned char *hashBytes = (unsigned char *) testHash;
    std::vector<unsigned char> hash(testHash, testHash + 16);
    std::vector<unsigned char> sign(128, 0x55);
    Resource resource("name1", size, hash, sign);
    Resource resource2("name2", size, hash, sign);
    Resource resource3("name3", size, hash, sign);
    Resource resource4("name4", size, hash, sign);
    Resource resource5("name5", size, hash, sign);
    std::unordered_map<std::string, std::vector<Resource>> resources;
    resources[publicKey] = std::vector<Resource>{resource, resource2};
    resources[publicKey2] = std::vector<Resource>{resource3, resource4, resource5};
    BroadcastMessage message(resources);
    auto converted = message.toByteStream();
    BroadcastMessage fromBytes = BroadcastMessage::fromByteStream(converted);
    Resource res = fromBytes.getResources().find(publicKey2)->second[1];
    assert(res.getName() == "name4");
    assert(res.getSize() == size);
    assert(res.getHash() == hash);
    assert(res.getSign() == sign);
    std::cout << "asserts passed" << std::endl;
}
*/
