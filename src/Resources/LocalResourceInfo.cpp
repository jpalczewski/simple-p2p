//
// Created by kamil on 16/05/17.
//

#include "LocalResourceInfo.h"

std::string LocalResourceInfo::toString() const
{
    const std::string string = "State: " + ::toString(state) + "\n";
    return string;
}

