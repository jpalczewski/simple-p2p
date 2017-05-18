//
// Created by kamil on 18/05/17.
//

#ifndef SIMPLE_P2P_UDPLISTENER_H
#define SIMPLE_P2P_UDPLISTENER_H


#include "Network/Socket.h"

class UdpListener
{
public:
    UdpListener(int listenPort);

    void start();
private:
    Socket socket;
};


#endif //SIMPLE_P2P_UDPLISTENER_H
