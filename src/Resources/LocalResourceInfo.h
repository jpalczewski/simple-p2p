//
// Created by kamil on 16/05/17.
//

#ifndef SIMPLE_P2P_LOCALRESOURCEINFO_H
#define SIMPLE_P2P_LOCALRESOURCEINFO_H

#include <string>
#include "Resource.h"

class LocalResourceInfo
{
public:
    enum class State {Active, Blocked, Invalid};
    std::string toString() const;
    uint64_t getLocalId() const;

private:
    Resource::State state;

    // TODO: more resource properties
};


#endif //SIMPLE_P2P_LOCALRESOURCEINFO_H
