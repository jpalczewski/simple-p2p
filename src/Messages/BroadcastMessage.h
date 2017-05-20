//
// Created by kamil on 20/05/17.
//

#ifndef SIMPLE_P2P_BROADCASTMESSAGE_H
#define SIMPLE_P2P_BROADCASTMESSAGE_H

#include "../Resources/Resource.h"
#include "MessageType.h"

class BroadcastMessage
{
public:
    BroadcastMessage(std::unordered_map<std::string, std::vector<Resource>> resources);
    static BroadcastMessage fromByteStream(std::vector<unsigned char> byteArray);

    std::vector<unsigned char> toByteStream() const;

    MessageType getMessageType();
    const std::unordered_map<std::string, std::vector<Resource>>& getResources() const;

private:
    const MessageType type = MessageType::BroadcastResource;
    const std::unordered_map<std::string, std::vector<Resource>> resources;
};


#endif //SIMPLE_P2P_BROADCASTMESSAGE_H
