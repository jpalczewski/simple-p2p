//
// Created by kamil on 20/05/17.
//

#include <vector>
#include <unordered_map>
#include "BroadcastMessage.h"
#include "../ConversionUtils.h"

BroadcastMessage::BroadcastMessage(std::unordered_map<std::string, std::vector<Resource>> resources) :
        resources(resources)
{
}

std::vector<unsigned char> BroadcastMessage::toByteStream() const
{
    std::vector<unsigned char> byteArray;
    byteArray.push_back(static_cast<unsigned char>(type));
    intToBytes(resources.size(), byteArray);
    for (const auto& entry : resources)
    {
        const char *key = entry.first.data();
        byteArray.insert(byteArray.end(), key, key + entry.first.length());
        intToBytes(entry.second.size(), byteArray);
        for (const auto& resource : entry.second)
        {
            resource.toByteStream(byteArray);
        }
    }
    return byteArray;
}


const std::unordered_map<std::string, std::vector<Resource>>& BroadcastMessage::getResources() const
{
    return resources;
}

MessageType BroadcastMessage::getMessageType()
{
    return type;
}

BroadcastMessage BroadcastMessage::fromByteStream(std::vector<unsigned char> byteArray)
{
    const auto type = static_cast<MessageType>(byteArray[0]);
    if (type != MessageType::BroadcastResource)
        throw std::runtime_error("Invalid message type to construct a BroadcastMessage from byte stream");
    std::unordered_map<std::string, std::vector<Resource>> resources;
    const int publicKeyCount = intFromBytes(byteArray, 1);
    int currentIndex = 5; // 1B - message type, 4B - number of public keys
    for (int i = 0; i < publicKeyCount; ++i)
    {
        const std::string publicKey(reinterpret_cast<const char*>(&byteArray[currentIndex]), 251);
        currentIndex += 251;
        const int resourceCount = intFromBytes(byteArray, currentIndex);
        currentIndex += 4;
        std::vector<Resource> publicKeyResources;
        for (int i = 0; i < resourceCount; ++i)
        {
            const Resource resource = Resource::fromByteStream(byteArray, currentIndex);
            publicKeyResources.push_back(std::move(resource));
        }
        resources[std::move(publicKey)] = std::move(publicKeyResources);
    }
    return BroadcastMessage(std::move(resources));
}


