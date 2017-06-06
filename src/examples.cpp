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