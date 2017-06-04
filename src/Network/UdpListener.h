//
// Created by kamil on 18/05/17.
//

#ifndef SIMPLE_P2P_UDPLISTENER_H
#define SIMPLE_P2P_UDPLISTENER_H


#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include "Socket.h"
#include "IpAddress.h"
#include "../Messages/MessageType.h"
#include "../Resources/Resource.h"
#include "../Messages/ResourceManagementMessage.h"

class UdpListener
{
public:
    UdpListener(int listenPort);

    void start();
private:
    Socket socket;

    const int bufferSize = 2048;

    void handleBroadcastMessage(std::vector<unsigned char> buffer, const IpAddress &sender) const;

    void handleResourceManagementMessage(std::vector<unsigned char> buffer, MessageType type) const;

    const std::unordered_map<MessageType, Resource::State, EnumHash> StateForMessage
    {
            {MessageType::BlockResource, Resource::State::Blocked},
            {MessageType::InvalidateResource, Resource::State::Invalid},
            {MessageType::UnblockResource, Resource::State::Active}
    };

    void handleDelete(ResourceManagementMessage message) const;
};


#endif //SIMPLE_P2P_UDPLISTENER_H
