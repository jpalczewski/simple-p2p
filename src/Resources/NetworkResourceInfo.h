//
// Created by kamil on 14/05/17.
//

#ifndef SIMPLE_P2P_RESOURCEINFO_H
#define SIMPLE_P2P_RESOURCEINFO_H

#include <vector>
#include <set>
#include "../Network/IpAddress.h"

class NetworkResourceInfo
{
public:
    enum class State {Active, Blocked, Invalid};

    void addSeeders(const std::set<IpAddress>& seeders);
    const std::set<IpAddress>& getSeeders() const;

private:
    State state;
    std::set<IpAddress> seeders;
    // TODO a list of nodes(IP addresses) with this resource? threads which send this networkResources to other nodes?
};


#endif //SIMPLE_P2P_RESOURCEINFO_H
