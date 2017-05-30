//
// Created by kamil on 28/05/17.
//

#ifndef SIMPLE_P2P_DAEMONCLIENT_H
#define SIMPLE_P2P_DAEMONCLIENT_H

#include <string>
#include "DaemonClientResponse.h"
#include "../Network/Socket.h"

class DaemonClient
{
public:
    DaemonClient(std::string daemonAddress, int daemonPort);

    DaemonClientResponse sendAdd(std::string filePath);
    DaemonClientResponse sendBroadcast();
    DaemonClientResponse sendDisplay();
    DaemonClientResponse sendDownload(uint64_t localId);

private:
    Socket socket;

    std::string daemonAddress;
    int daemonPort;

    DaemonClientResponse sendMessage(std::string message);
};

#endif //SIMPLE_P2P_DAEMONCLIENT_H
