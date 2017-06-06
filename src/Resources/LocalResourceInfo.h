//
// Created by kamil on 16/05/17.
//

#ifndef SIMPLE_P2P_LOCALRESOURCEINFO_H
#define SIMPLE_P2P_LOCALRESOURCEINFO_H

#include <string>
#include "Resource.h"
#include "GenericResourceInfo.h"

class LocalResourceInfo : public GenericResourceInfo
{
public:
    enum class State {Active, Blocked, Invalid};

    LocalResourceInfo(uint64_t localId);
    LocalResourceInfo() = default;
    std::string toString() const;
    //uint64_t getLocalId() const;

private:
};


#endif //SIMPLE_P2P_LOCALRESOURCEINFO_H
