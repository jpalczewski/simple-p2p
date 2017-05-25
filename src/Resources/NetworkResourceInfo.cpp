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

std::string toString(NetworkResourceInfo::State state)
{
    switch (state)
    {
        case NetworkResourceInfo::State::Active:
            return "active";
        case NetworkResourceInfo::State::Blocked:
            return "blocked";
        case NetworkResourceInfo::State::Invalid:
            return "invalid";
    }
}

std::string NetworkResourceInfo::toString() const
{
    // TODO reserve place for a string in advance
    std::string string = "State: " + ::toString(state) + "\n";
    string += "Seeders:\n";
    for (const auto& seeder : seeders)
    {
        string += seeder.toString() + "\n";
    }
    return string;
}





