//
// Created by kamil on 27/05/17.
//

#include "ResourceRequestMessage.h"
#include "../ConversionUtils.h"


ResourceRequestMessage::ResourceRequestMessage(AuthorKeyType publicKey, const Resource &resource, int64_t offset, int64_t size)
        : publicKey(std::move(publicKey)),
          resource(resource),
          offset(offset),
          size(size)
{ }

ResourceRequestMessage ResourceRequestMessage::fromByteStream(std::vector<unsigned char> byteArray)
{
    const auto type = static_cast<MessageType>(byteArray[0]);
    if (type != MessageType::ResourceRequest)
        throw std::runtime_error("Invalid message type to construct a ResourceRequestMessage from byte stream");
    int index = 1;
    Resource resource = Resource::fromByteStream(byteArray, index);
    AuthorKeyType publicKey(reinterpret_cast<const char*>(&byteArray[index]), 251);
    index += 251;
    const int64_t offset = int64FromBytes(byteArray, index);
    index += 8;
    const int64_t size = int64FromBytes(byteArray, index);
    return ResourceRequestMessage(std::move(publicKey), std::move(resource), offset, size);
}

std::vector<unsigned char> ResourceRequestMessage::toByteStream() const
{
    std::vector<unsigned char> byteArray;
    byteArray.push_back(static_cast<unsigned char>(type));
    resource.toByteStream(byteArray);
    byteArray.insert(byteArray.end(), publicKey.begin(), publicKey.end());
    int64ToBytes(offset, byteArray);
    int64ToBytes(size, byteArray);
    return byteArray;
}

const int64_t ResourceRequestMessage::getSize() const
{
    return size;
}

const int64_t ResourceRequestMessage::getOffset() const
{
    return offset;
}

const Resource &ResourceRequestMessage::getResource() const
{
    return resource;
}

const MessageType ResourceRequestMessage::getType() const
{
    return type;
}

const AuthorKeyType &ResourceRequestMessage::getPublicKey() const
{
    return publicKey;
}



