//
// Created by kamil on 27/05/17.
//

#include "SendResourceMessage.h"
#include "../ConversionUtils.h"


SendResourceMessage::SendResourceMessage(const Resource &resource, int64_t offset, int64_t size, std::vector<unsigned char> data)
        : resource(resource), offset(offset),
          size(size), data(std::move(data))
{ }

SendResourceMessage SendResourceMessage::fromByteStream(std::vector<unsigned char> byteArray)
{
    const auto type = static_cast<MessageType>(byteArray[0]);
    if (type != MessageType::SendResource)
        throw std::runtime_error("Invalid message type to construct a SendRequestMessage from byte stream");
    int index = 1;
    Resource resource = Resource::fromByteStream(byteArray, index);
    const int64_t offset = int64FromBytes(byteArray, index);
    index += 8;
    const int64_t size = int64FromBytes(byteArray, index);
    index += 8;
    std::vector<unsigned char> data(byteArray.begin() + index, byteArray.begin() + index + size);
    return SendResourceMessage(std::move(resource), offset, size, std::move(data));
}

std::vector<unsigned char> SendResourceMessage::toByteStream() const
{
    std::vector<unsigned char> byteArray;
    byteArray.push_back(static_cast<unsigned char>(type));
    resource.toByteStream(byteArray);
    int64ToBytes(offset, byteArray);
    int64ToBytes(size, byteArray);
    byteArray.insert(byteArray.end(), data.begin(), data.end());
    return byteArray;
}

const int64_t SendResourceMessage::getSize() const
{
    return size;
}

const int64_t SendResourceMessage::getOffset() const
{
    return offset;
}

const Resource &SendResourceMessage::getResource() const
{
    return resource;
}

const std::vector<unsigned char> &SendResourceMessage::getData() const
{
    return data;
}

const MessageType SendResourceMessage::getType() const
{
    return type;
}