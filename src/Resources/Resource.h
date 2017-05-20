//
// Created by kamil on 14/05/17.
//

#ifndef SIMPLE_P2P_RESOURCE_H
#define SIMPLE_P2P_RESOURCE_H

#include <string>
#include <cstdint>
#include <vector>

#include <boost/functional/hash.hpp>

struct ResourceHash;

class Resource
{
public:
    void toByteStream(std::vector<unsigned char>& byteArray) const;

    bool operator==(const Resource& other)
    {
        return name == other.name && size == other.size && hash == other.hash && sign == other.sign;
    }

    static Resource fromByteStream(const std::vector<unsigned char>& vector, int index);

private:
    std::string name;
    int64_t size;
    std::vector<unsigned char> hash; // 16B
    std::vector<unsigned char> sign; // 128B

    friend class ResourceHash;
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
