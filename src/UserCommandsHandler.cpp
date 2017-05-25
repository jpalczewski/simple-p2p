//
// Created by kamil on 25/05/17.
//

#include <iostream>
#include "UserCommandsHandler.h"
#include "Messages/BroadcastMessage.h"

UserCommandsHandler::UserCommandsHandler(int port) : port(port), socket(Socket::Domain::Ip4, Socket::Type::Udp)
{
    socket.enableBroadcast();
}

UserCommandsHandler::~UserCommandsHandler()
{
    socket.close();
}

void UserCommandsHandler::handleUserInput()
{
    while (true)
    {
        std::cout << "Available commands: add, display, broadcast, exit";
        std::string command;
        std::cin >> command;
        if (command == "exit")
            exit(0);
        if (command == "add")
            handleAdd();
        if (command == "display")
            handleDisplay();
        if (command == "broadcast")
            handleBroadcast();
    }
}

void UserCommandsHandler::handleAdd()
{
    std::cout << "Please enter a file name." << std::endl;
    std::string fileName;
    std::cin >> fileName;
    Resource resource = resourceFromFile(fileName);
    std::string publicKey = readPublicKey();
    resourceManager.addOwnedResource(publicKey, resource);
    std::cout << "File " << fileName << "added to local resources.";
}

void UserCommandsHandler::handleDisplay()
{
    const auto resources = resourceManager.getNetworkResources();
    for (const auto &keyRes : resources)
    {
        std::cout << "key: " << std::endl;
        std::cout << keyRes.first << std::endl;
        for (const auto &res : keyRes.second)
        {
            std::cout << res.first.toString() << std::endl;
            std::cout << res.second.toString() << std::endl;
        }
        std::cout << std::endl;
    }
}

void UserCommandsHandler::handleBroadcast()
{
    // TODO fragmentation is a big deal, should divide resources into multiple packets to prevent it
    const auto ownedResources = resourceManager.getOwnedResources();
    const auto map = convertInfoMapToResourceMap(ownedResources);
    BroadcastMessage message(map);
    const auto bytes = message.toByteStream();
    std::cout << "--- Sending broadcast." << std::endl;
    socket.writeTo((const char *) &bytes[0], bytes.size(), "127.255.255.255", port);
    std::cout << std::to_string(bytes.size()) << "bytes sent." << std::endl;
}

Resource UserCommandsHandler::resourceFromFile(std::string fileName)
{
    int size = 1111;
    const char *testHash = "0123456789012345";
    unsigned char *hashBytes = (unsigned char *) testHash;
    std::vector<unsigned char> hash(testHash, testHash + 16);
    std::vector<unsigned char> sign(128, 0x55);
    return Resource(fileName, size, hash, sign);
}

std::string UserCommandsHandler::readPublicKey()
{
    return "-----BEGIN RSA PUBLIC KEY-----\n"
            "MIGJAoGBANLVDLEKgzjGBqiNPTqRqPQynEbbNZbY2T2K+Fn3YMX8hbZ8QLn48CZI\n"
            "n4mKDQSVND1qxBCeRBxYeaavwyZ8LCqVYPCXZwkRH0QlyReQOCxyw7duEIl/F7tc\n"
            "N3/34qht2DK8lOp9IQS7sY9L5Oy73JW4jQUMcPUUg52bq7pzccWFAgMBAAE=\n"
            "-----END RSA PUBLIC KEY-----\n";
}

std::unordered_map<std::string, std::vector<Resource>> UserCommandsHandler::convertInfoMapToResourceMap(ResourceManager::ResourceMap<LocalResourceInfo> map)
{
    std::unordered_map<std::string, std::vector<Resource>> allResources;
    for (const auto& keyResource : map)
    {
        std::vector<Resource> resources;
        for (const auto& resourceWithInfo : keyResource.second)
        {
            resources.push_back(resourceWithInfo.first);
        }
        allResources.insert(std::make_pair(keyResource.first, std::move(resources)));
    }
    return allResources;
}










