//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_BROADCASTCOMMAND_H
#define SIMPLE_P2P_BROADCASTCOMMAND_H

#include "Command.h"
#include "../Visitor.h"
#include "NoParamCommand.h"

class BroadcastCommand : public NoParamCommand
{
public:
    void accept(Visitor* visitor) {visitor->handle(this);};
    Type getType() {return Type::Broadcast;}
};

#endif //SIMPLE_P2P_BROADCASTCOMMAND_H
