//
// Created by kamil on 27/05/17.
//

#ifndef SIMPLE_P2P_RESOURCEREQUESTMESSAGE_H
#define SIMPLE_P2P_RESOURCEREQUESTMESSAGE_H


#include <vector>
#include "MessageType.h"
#include "../Resources/Resource.h"

class ResourceRequestMessage
{
public:
    ResourceRequestMessage(const Resource &resource, int64_t offset, int64_t size);

    static ResourceRequestMessage fromByteStream(std::vector<unsigned char> byteArray);
    std::vector<unsigned char> toByteStream() const;

    const MessageType getType() const;
    const Resource & getResource() const;
    const int64_t getOffset() const;
    const int64_t getSize() const;

private:
    const MessageType type = MessageType::BroadcastResource;

    const Resource resource;
    const int64_t offset;
    const int64_t size;
};

#endif //SIMPLE_P2P_RESOURCEREQUESTMESSAGE_H
