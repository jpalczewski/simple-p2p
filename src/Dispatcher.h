//
// Created by kamil on 05/05/17.
//

#ifndef SIMPLE_P2P_DISPATCHER_H
#define SIMPLE_P2P_DISPATCHER_H

#include "Handler.h"
#include "Network/Socket.h"

class Dispatcher {
public:
    Dispatcher(int listenPort, int maximumConnections);

    void start();
private:
    Socket socket;
    Handler handler;
};


#endif //SIMPLE_P2P_DISPATCHER_H
