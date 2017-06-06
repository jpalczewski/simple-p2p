//
// Created by kamil on 16/05/17.
//

#include "LocalResourceInfo.h"

std::string LocalResourceInfo::toString() const
{
    std::stringstream ss;
    ss << "\tState: " << ::toString(getResourceState()) << std::endl;
    //std::string string = "State: " + ::toString(getResourceState()) + "\n";
    return ss.str();
}

LocalResourceInfo::LocalResourceInfo(uint64_t localId) : GenericResourceInfo(localId) {}

//uint64_t LocalResourceInfo::getLocalId() const
//{
//    return 0;
//}



