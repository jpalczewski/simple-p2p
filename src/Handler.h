//
// Created by kamil on 05/05/17.
//

#ifndef SIMPLE_P2P_HANDLER_H
#define SIMPLE_P2P_HANDLER_H

#include "Network/Socket.h"
#include "Messages/ResourceRequestMessage.h"

class Handler {
public:
    void handle(Socket connection);

private:
    void processResourceRequest(std::vector<unsigned char> vector, Socket &connection);

    void readBytes(Socket &socket, std::vector<unsigned char>& buffer, const int size);

    void processResourceRequestMessage(ResourceRequestMessage message, Socket& connection);
};


#endif //SIMPLE_P2P_HANDLER_H
