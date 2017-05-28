//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_ADDCOMMAND_H
#define SIMPLE_P2P_ADDCOMMAND_H

#include <string>
#include <boost/serialization/access.hpp>
#include "Command.h"
#include "Visitor.h"

class AddCommand : public Command
{
public:
    AddCommand(std::string fileName) : fileName(std::move(fileName)) { }
    AddCommand() = default;
    void accept(Visitor* visitor) {visitor->handle(this);};
    Type getType() {return Type::Add;}
    const std::string& getFileName() { return fileName; }

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & fileName;
    }
private:
    std::string fileName;
    friend class boost::serialization::access;
};

#endif //SIMPLE_P2P_ADDCOMMAND_H
