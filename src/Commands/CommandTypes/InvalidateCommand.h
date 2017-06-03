//
// Created by erxyi on 03.06.17.
//

#ifndef SIMPLE_P2P_INVALIDATECOMMAND_H
#define SIMPLE_P2P_INVALIDATECOMMAND_H


#include "OneIntegerParamCommand.h"

class InvalidateCommand : public OneIntegerParamCommand {
public:
    InvalidateCommand(uint64_t localId) : OneIntegerParamCommand(localId) {}

    InvalidateCommand() {}

    Type getType() override {
        return Command::Type::Invalidate;
    }

};


#endif //SIMPLE_P2P_INVALIDATECOMMAND_H
