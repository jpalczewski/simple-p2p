//
// Created by kamil on 14/05/17.
//

#include "NetworkResourceInfo.h"

void NetworkResourceInfo::addSeeders(const std::set<IpAddress> &seeders)
{
    for (const auto& seeder : seeders)
    {
        auto found = this->seeders.find(seeder);
        if (found == this->seeders.end())
            this->seeders.insert(found, seeder);
    }
}

const std::set<IpAddress>& NetworkResourceInfo::getSeeders() const
{
    return this->seeders;
}



