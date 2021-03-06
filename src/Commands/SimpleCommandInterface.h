//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_SIMPLECOMMANDINTERFACE_H
#define SIMPLE_P2P_SIMPLECOMMANDINTERFACE_H

#include "CommandInterface.h"

class SimpleCommandInterface : public CommandInterface
{
public:
    std::unique_ptr<Command> getNextCommand();
    void sendResponse(std::string response);

private:
    std::string getFileName();
};


#endif //SIMPLE_P2P_SIMPLECOMMANDINTERFACE_H
