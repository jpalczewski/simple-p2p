//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_COMMAND_H
#define SIMPLE_P2P_COMMAND_H

#include "Visitable.h"

class Command : public Visitable
{
public:
    enum class Type : unsigned char {Broadcast, Display, Add, Unknown, Download };
    virtual Type getType() = 0;
};

#endif //SIMPLE_P2P_COMMAND_H
