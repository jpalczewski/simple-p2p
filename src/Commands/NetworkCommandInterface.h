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
    NetworkCommandInterface();

    std::unique_ptr<Command> getNextCommand();
    void sendResponse(std::string response);

private:
    Socket socket;
    const int listenPort = 6000;
    IpAddress lastSender;
};

#endif //SIMPLE_P2P_NETWORKCOMMANDINTERFACE_H
