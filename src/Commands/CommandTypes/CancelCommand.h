//
// Created by erxyi on 03.06.17.
//

#ifndef SIMPLE_P2P_CANCELCOMMAND_H
#define SIMPLE_P2P_CANCELCOMMAND_H


#include "OneIntegerParamCommand.h"

class CancelCommand : public OneIntegerParamCommand {
public:
    CancelCommand(uint64_t localId) : OneIntegerParamCommand(localId) {}
    CancelCommand() {}
    void accept(Visitor* visitor) {visitor->handle(this);};
    Type getType() override {
        return Command::Type::Cancel;
    }

};


#endif //SIMPLE_P2P_CANCELCOMMAND_H
