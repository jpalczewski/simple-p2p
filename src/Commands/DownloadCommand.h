//
// Created by kamil on 30/05/17.
//

#ifndef SIMPLE_P2P_DOWNLOADCOMMAND_H
#define SIMPLE_P2P_DOWNLOADCOMMAND_H

#include <cstdint>
#include <boost/serialization/access.hpp>
#include "Visitor.h"
#include "Command.h"

class DownloadCommand : public Command
{
public:
    DownloadCommand(uint64_t localId) : localId(localId) { }
    DownloadCommand() = default;
    void accept(Visitor* visitor) {visitor->handle(this);};
    Type getType() {return Type::Download;}
    uint64_t getLocalId() { return localId; }

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & localId;
    }
private:
    uint64_t localId;
    friend class boost::serialization::access;
};


#endif //SIMPLE_P2P_DOWNLOADCOMMAND_H
