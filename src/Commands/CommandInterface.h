//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_COMMANDINTERFACE_H
#define SIMPLE_P2P_COMMANDINTERFACE_H

#include <memory>

class Command;

class CommandInterface
{
public:
    virtual std::unique_ptr<Command> getNextCommand() = 0;
    virtual void sendResponse(std::string response) = 0;
};

#endif //SIMPLE_P2P_COMMANDINTERFACE_H
