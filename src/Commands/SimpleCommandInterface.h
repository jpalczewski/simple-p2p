//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_SIMPLECOMMANDINTERFACE_H
#define SIMPLE_P2P_SIMPLECOMMANDINTERFACE_H

#include "CommandInterface.h"

// TODO better class name...
class SimpleCommandInterface : public CommandInterface
{
public:
    std::unique_ptr<Command> getNextCommand();

private:
    std::string getFileName();
};


#endif //SIMPLE_P2P_SIMPLECOMMANDINTERFACE_H
