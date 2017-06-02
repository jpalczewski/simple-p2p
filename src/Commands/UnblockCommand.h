//
// Created by erxyi on 03.06.17.
//

#ifndef SIMPLE_P2P_UNBLOCKCOMMAND_H
#define SIMPLE_P2P_UNBLOCKCOMMAND_H


#include "OneIntegerParamCommand.h"

class UnblockCommand : public OneIntegerParamCommand {
public:
    UnblockCommand(uint64_t localId) : OneIntegerParamCommand(localId) {}
    UnblockCommand() {}

    Type getType() override {
        return Command::Type::Unblock;
    }

};


#endif //SIMPLE_P2P_UNBLOCKCOMMAND_H
