//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_ADDCOMMAND_H
#define SIMPLE_P2P_ADDCOMMAND_H

#include <string>
#include "Command.h"
#include "Visitor.h"

class AddCommand : public Command
{
public:
    AddCommand(const std::string &fileName) : fileName(fileName) { }
    void accept(Visitor* visitor) {visitor->handle(this);};
    Type getType() {return Type::Add;}
    const std::string& getFileName() { return fileName; }
private:
    std::string fileName;
};


#endif //SIMPLE_P2P_ADDCOMMAND_H
