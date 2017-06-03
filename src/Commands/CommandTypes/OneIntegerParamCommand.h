//
// Created by erxyi on 02.06.17.
//

#ifndef SIMPLE_P2P_ONEPARAMCOMMAND_H
#define SIMPLE_P2P_ONEPARAMCOMMAND_H
#include <boost/serialization/access.hpp>
#include "../Visitor.h"
#include "Command.h"
#include <cstdint>

class OneIntegerParamCommand : public Command
{
public:
    OneIntegerParamCommand(uint64_t localId) : localId(localId) { }
    OneIntegerParamCommand() = default;
    void accept(Visitor* visitor) {visitor->handle(this);};
    virtual Type getType() = 0;
    uint64_t getLocalId() {return localId; }

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & localId;
    }

private:
    uint64_t localId;
    friend class boost::serialization::access;
};

#endif //SIMPLE_P2P_ONEPARAMCOMMAND_H
