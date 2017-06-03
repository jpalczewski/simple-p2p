//
// Created by kamil on 30/05/17.
//

#ifndef SIMPLE_P2P_DOWNLOADCOMMAND_H
#define SIMPLE_P2P_DOWNLOADCOMMAND_H

#include <cstdint>
#include <boost/serialization/access.hpp>
#include "../Visitor.h"
#include "Command.h"
#include "OneIntegerParamCommand.h"

class DownloadCommand : public OneIntegerParamCommand
{
public:
    DownloadCommand(uint64_t localId) : OneIntegerParamCommand(localId) {}
    void accept(Visitor* visitor) {visitor->handle(this);};
    DownloadCommand() {}

    Command::Type getType() {return Command::Type::Download;}
};


#endif //SIMPLE_P2P_DOWNLOADCOMMAND_H
