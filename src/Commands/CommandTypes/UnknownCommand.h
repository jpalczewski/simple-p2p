//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_UNKNOWNCOMMAND_H
#define SIMPLE_P2P_UNKNOWNCOMMAND_H

#include <string>
#include "Command.h"
#include "../Visitor.h"

class UnknownCommand: public Command
{
public:
    UnknownCommand() : command("lol i'm unknown") {}
    UnknownCommand(const std::string &command) : command(command) { }
    void accept(Visitor* visitor) {visitor->handle(this);};
    Type getType() {return Type::Unknown;}
    const std::string& getCommand() { return command; }
private:
    std::string command;
};

#endif //SIMPLE_P2P_UNKNOWNCOMMAND_H
