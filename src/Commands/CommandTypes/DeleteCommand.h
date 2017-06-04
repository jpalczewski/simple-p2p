//
// Created by erxyi on 03.06.17.
//

#ifndef SIMPLE_P2P_DELETE_H
#define SIMPLE_P2P_DELETE_H


#include "OneIntegerParamCommand.h"

class DeleteCommand : public OneIntegerParamCommand {
public:
    DeleteCommand(uint64_t localId) : OneIntegerParamCommand(localId) {}
    void accept(Visitor* visitor) {visitor->handle(this);};
    DeleteCommand() {}

    Type getType() override {
        return Command::Type::Delete;
    }

};


#endif //SIMPLE_P2P_DELETE_H
