//
// Created by kamil on 18/05/17.
//

#include <iostream>
#include "UdpListener.h"
#include "Messages/MessageType.h"

UdpListener::UdpListener(int listenPort) : socket(Socket::Domain::Ip4, Socket::Type::Udp)
{
    socket.bind(listenPort);
}

void UdpListener::start()
{
    char buffer[1024];
    while (true)
    {
        std::string sender;
        int received = socket.readFrom(buffer, 1, sender);
        if (buffer[0] == (char) MessageType::BroadcastResource)
            std::cout << "Received broadcast" << std::endl;
    }
}



