//
// Created by kamil on 16/05/17.
//

#include "LocalResourceInfo.h"

std::string LocalResourceInfo::toString() const
{
    const std::string string = "State: " + ::toString(state) + "\n";
    return string;
}

uint64_t LocalResourceInfo::getLocalId() const
{
    // TODO implement when needed. It would be used to identify resources to delete/block
    return 0;
}



