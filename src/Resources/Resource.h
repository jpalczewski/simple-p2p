//
// Created by kamil on 14/05/17.
//

#ifndef SIMPLE_P2P_RESOURCE_H
#define SIMPLE_P2P_RESOURCE_H

#include <string>
#include <stdint.h>
#include <vector>

#include <boost/functional/hash.hpp>

class Resource
{
public:
    std::string name;
    int64_t size;
    std::vector<unsigned char> hash; // 16B
    std::vector<unsigned char> sign; // 128B

    bool operator==(const Resource& other)
    {
        return name == other.name && size == other.size && hash == other.hash && sign == other.sign;
    }
};

// would be used to place Resource as a map key
struct ResourceHash
{
    size_t operator()(Resource resource) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, resource.name);
        boost::hash_combine(seed, resource.size);
        boost::hash_combine(seed, resource.hash);
        boost::hash_combine(seed, resource.sign);
        return seed;
    }
};


#endif //SIMPLE_P2P_RESOURCE_H
