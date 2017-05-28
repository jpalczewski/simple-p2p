//
// Created by kamil on 18/05/17.
//

#include <iostream>
#include "UdpListener.h"
#include "../Resources/ResourceManager.h"
#include "../Messages/BroadcastMessage.h"

UdpListener::UdpListener(int listenPort) : socket(Socket::Domain::Ip4, Socket::Type::Udp)
{
    socket.bind(listenPort);
}

void UdpListener::start()
{
    const int size = 1024;
    unsigned char buffer[size];
    while (true)
    {
        // TODO make it multithreaded and remember sender ip in resource info
        // and handle messages bigger than buffer
        // and refactor it, generally
        std::string sender;
        int received = socket.readFrom(buffer, size, sender);
        if (buffer[0] == (char) MessageType::BroadcastResource)
        {
            std::cout << "Received broadcast from " + sender << std::endl;

            std::cout << std::to_string(received) << " bytes received" << std::endl;
            if (received == size)
                throw std::runtime_error("buffer too small");
            std::vector<unsigned char> bytes = std::vector<unsigned char>(buffer, buffer + received);
            BroadcastMessage message = BroadcastMessage::fromByteStream(std::move(bytes));
            for (const auto& keyResource : message.getResources())
            {
                for (const auto &resource : keyResource.second)
                {
                    NetworkResourceInfo info{};
                    info.addSeeders(std::set<IpAddress>{sender});
                    resourceManager.addNetworkResource(keyResource.first, resource, info);
                }
            }
        }
    }
}



