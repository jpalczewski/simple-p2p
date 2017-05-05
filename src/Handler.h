//
// Created by kamil on 05/05/17.
//

#ifndef SIMPLE_P2P_HANDLER_H
#define SIMPLE_P2P_HANDLER_H

#include "Network/Socket.h"

class Handler {
public:
    void handle(Socket connection);
};


#endif //SIMPLE_P2P_HANDLER_H
