//
// Created by kamil on 05/05/17.
//

#include <iostream>
#include "Handler.h"
#include "Messages/MessageType.h"

void Handler::handle(Socket connection)
{
    char buffer[1024];
    int received = connection.read(buffer, 1);
    if (buffer[0] == (char) MessageType::BroadcastResource)
        std::cout << "Received broadcast"; // nvm, tcp connection cannot receive broadcast
}