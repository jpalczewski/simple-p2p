//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_DISPLAYCOMMAND_H
#define SIMPLE_P2P_DISPLAYCOMMAND_H

#include "Command.h"
#include "../Visitor.h"
#include "NoParamCommand.h"

class DisplayCommand : public NoParamCommand
{
public:
    void accept(Visitor* visitor) {visitor->handle(this);};
    Type getType() {return Type::Display;}
};


#endif //SIMPLE_P2P_DISPLAYCOMMAND_H
