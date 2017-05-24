//
// Created by kamil on 24/05/17.
//

#include "IpAddress.h"

bool operator<(const IpAddress& left, const IpAddress& right)
{
    return left.address < right.address;
}