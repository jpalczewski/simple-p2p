//
// Created by erxyi on 03.06.17.
//

#ifndef SIMPLE_P2P_STATUSCOMMAND_H
#define SIMPLE_P2P_STATUSCOMMAND_H


#include "NoParamCommand.h"

class StatusCommand : public NoParamCommand {
public:
    Type getType() override {
        return Command::Type::Status;
    }

    void accept(Visitor *visitor) override {

    }

};


#endif //SIMPLE_P2P_STATUSCOMMAND_H
