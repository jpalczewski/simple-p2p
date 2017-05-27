//
// Created by kamil on 27/05/17.
//

#ifndef SIMPLE_P2P_SENDRESOURCEMESSAGE_H
#define SIMPLE_P2P_SENDRESOURCEMESSAGE_H


#include "../Resources/Resource.h"
#include "MessageType.h"

class SendResourceMessage
{
public:
    SendResourceMessage(const Resource &resource, int64_t offset, int64_t size, std::vector<unsigned char> data);

    static SendResourceMessage fromByteStream(std::vector<unsigned char> byteArray);
    std::vector<unsigned char> toByteStream() const;

    const MessageType getType() const;
    const Resource & getResource() const;
    const int64_t getOffset() const;
    const int64_t getSize() const;
    const std::vector<unsigned char>& getData() const;

private:
    const MessageType type = MessageType::SendResource;

    const Resource resource;
    const int64_t offset;
    const int64_t size;
    const std::vector<unsigned char> data;
};

#endif //SIMPLE_P2P_SENDRESOURCEMESSAGE_H
