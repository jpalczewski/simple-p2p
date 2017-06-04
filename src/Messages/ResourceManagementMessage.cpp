//
// Created by kamil on 04/06/17.
//

#include "ResourceManagementMessage.h"

ResourceManagementMessage ResourceManagementMessage::fromByteStream(std::vector<unsigned char> byteArray, int currentIndex)
{
    Resource resource = Resource::fromByteStream(byteArray, currentIndex);
    std::string publicKey(reinterpret_cast<const char*>(&byteArray[currentIndex]), 251);
    currentIndex += 251;
    const std::vector<unsigned char> sign(&byteArray[currentIndex], &byteArray[currentIndex + 128]);
    currentIndex += 128;
    return ResourceManagementMessage(std::move(publicKey), std::move(resource), std::move(sign));
}

std::vector<unsigned char> ResourceManagementMessage::toByteStream() const
{
    std::vector<unsigned char> byteArray;
    resource.toByteStream(byteArray);
    byteArray.insert(byteArray.end(), publicKey.begin(), publicKey.end());
    byteArray.insert(byteArray.end(), sign.begin(), sign.end());
    return byteArray;
}



