//
// Created by kamil on 14/05/17.
//

#ifndef SIMPLE_P2P_RESOURCEINFO_H
#define SIMPLE_P2P_RESOURCEINFO_H

#include <vector>
#include "../Network/IpAddress.h"

class NetworkResourceInfo
{
public:
    enum class State {Active, Blocked, Invalid};

    State state;
    std::vector<IpAddress> seeders;
    // TODO a list of nodes(IP addresses) with this resource? threads which send this networkResources to other nodes?
};


#endif //SIMPLE_P2P_RESOURCEINFO_H
