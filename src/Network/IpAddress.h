//
// Created by kamil on 23/05/17.
//

#ifndef SIMPLE_P2P_IPADDRESS_H
#define SIMPLE_P2P_IPADDRESS_H

#include <string>

class IpAddress
{
public:
    IpAddress(const std::string &address) : address(address), port(0)
    { }
    IpAddress() = default;

    friend bool operator<(const IpAddress& left, const IpAddress& right);

    std::string getAddress() const;

    int getPort() const
    {
        return port;
    }

    std::string toString() const;

    void setAddress(const std::string &address)
    {
        IpAddress::address = address;
    }

    void setPort(int port)
    {
        IpAddress::port = port;
    }

private:
    std::string address; // TODO maybe some better representation? bytes or whatever
    int port;
};

#endif //SIMPLE_P2P_IPADDRESS_H
