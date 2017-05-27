//
// Created by kamil on 25/05/17.
//

#include "UserCommandsHandler.h"
#include <iostream>
#include "../Messages/BroadcastMessage.h"
#include "AddCommand.h"
#include "SimpleCommandInterface.h"

UserCommandsHandler::UserCommandsHandler(int port) : port(port), socket(Socket::Domain::Ip4, Socket::Type::Udp),
                                                     commandInterface(std::make_unique<SimpleCommandInterface>())
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
        std::unique_ptr<Command> command = commandInterface->getNextCommand();
        command->accept(this);
    }
}

template <typename InfoType>
void printResourceMap(const ResourceManager::ResourceMap<InfoType>& resources)
{
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

void UserCommandsHandler::handle(BroadcastCommand *command)
{
    // TODO fragmentation is a big deal, should divide resources into multiple packets to prevent it
    const auto ownedResources = resourceManager.getOwnedResources();
    const auto map = convertInfoMapToResourceMap(ownedResources);
    BroadcastMessage message(map);
    const auto bytes = message.toByteStream();
    std::cout << "--- Sending broadcast." << std::endl;
    socket.writeTo(&bytes[0], bytes.size(), "127.255.255.255", port);
    std::cout << std::to_string(bytes.size()) << "bytes sent." << std::endl;
}

void UserCommandsHandler::handle(DisplayCommand *command)
{
    std::cout << "Network resources: " << std::endl;
    const auto resources = resourceManager.getNetworkResources();
    printResourceMap<NetworkResourceInfo>(resources);
    std::cout << "Owned resources: " << std::endl;
    const auto ownedResources = resourceManager.getOwnedResources();
    printResourceMap<LocalResourceInfo>(ownedResources);
}

void UserCommandsHandler::handle(AddCommand *command)
{
    Resource resource = resourceFromFile(command->getFileName());
    std::string publicKey = readPublicKey();
    resourceManager.addOwnedResource(publicKey, resource);
    std::cout << "File " << command->getFileName() << " added to local resources." << std::endl;
}

void UserCommandsHandler::handle(UnknownCommand *command)
{
    std::cout << "Unknown command." << std::endl;
}

















