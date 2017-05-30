//
// Created by kamil on 28/05/17.
//

#ifndef SIMPLE_P2P_NETWORKCOMMANDINTERFACE_H
#define SIMPLE_P2P_NETWORKCOMMANDINTERFACE_H


#include "CommandInterface.h"
#include "../Network/Socket.h"

class NetworkCommandInterface : public CommandInterface
{
public:
    NetworkCommandInterface(int listenPort);

    std::unique_ptr<Command> getNextCommand();
    void sendResponse(std::string response);

private:
    Socket socket;
    const int listenPort;
    IpAddress lastSender;
};

#endif //SIMPLE_P2P_NETWORKCOMMANDINTERFACE_H
