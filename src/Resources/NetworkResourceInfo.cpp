//
// Created by kamil on 14/05/17.
//

#include "NetworkResourceInfo.h"

void NetworkResourceInfo::addSeeders(const std::vector<IpAddress> &seeders)
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

std::string NetworkResourceInfo::toString() const
{
    std::stringstream ss;
    ss << "\tState:" << ::toString(getResourceState()) << std::endl
       << "\tSeeders:" << std::endl;
    for (const auto& seeder : seeders)
    {
        ss << "\t\t" << seeder.toString() << std::endl;
    }
    return ss.str();
}





