//
// Created by kamil on 14/05/17.
//

#ifndef SIMPLE_P2P_RESOURCEINFO_H
#define SIMPLE_P2P_RESOURCEINFO_H

#include <vector>
#include <set>
#include "../Network/IpAddress.h"
#include "Resource.h"

class NetworkResourceInfo
{
public:

    void addSeeders(const std::set<IpAddress>& seeders);
    const std::set<IpAddress>& getSeeders() const;

    std::string toString() const;

private:
    Resource::State state;
    std::set<IpAddress> seeders;
    // TODO a list of nodes(IP addresses) with this resource? threads which send this networkResources to other nodes?
};
#endif //SIMPLE_P2P_RESOURCEINFO_H
