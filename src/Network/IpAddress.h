//
// Created by kamil on 23/05/17.
//

#ifndef SIMPLE_P2P_IPADDRESS_H
#define SIMPLE_P2P_IPADDRESS_H

#include <string>

struct IpAddress
{
    IpAddress(const std::string &address) : address(address)
    { }
    IpAddress() = default;

    friend bool operator<(const IpAddress& left, const IpAddress& right);

    std::string address; // TODO maybe some better representation? bytes or whatever
};

#endif //SIMPLE_P2P_IPADDRESS_H
