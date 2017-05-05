//
// Created by kamil on 05/05/17.
//

#include <iostream>
#include "Handler.h"

void Handler::handle(Socket connection)
{
    char buffer[1024];
    while (true)
    {
        int received = connection.read(buffer, 1024);
        std::string response = "[server response] Received: " + std::string(buffer, received) + "\n";
        std::cout << "Received from client: "  + std::string(buffer, received) << std::endl;
        connection.write(response.c_str(), response.length());
    }
}