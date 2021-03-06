//
// Created by kamil on 14/05/17.
//

#ifndef SIMPLE_P2P_RESOURCEINFO_H
#define SIMPLE_P2P_RESOURCEINFO_H

#include <vector>
#include <set>
#include <cstdint>
#include "../Network/IpAddress.h"
#include "Resource.h"
#include "GenericResourceInfo.h"

class NetworkResourceInfo : public GenericResourceInfo
{
public:
    NetworkResourceInfo(uint64_t localId) :
            GenericResourceInfo(localId)
    { }
    NetworkResourceInfo(const NetworkResourceInfo& other) = default;

    void addSeeders(const std::vector<IpAddress>& seeders);

    const std::set<IpAddress>& getSeeders() const;

    std::string toString() const;

private:
    std::set<IpAddress> seeders;
};

#endif //SIMPLE_P2P_RESOURCEINFO_H
