//
// Created by kamil on 14/05/17.
//

#include "Resource.h"
#include "../ConversionUtils.h"

Resource::Resource(const std::string &name, int64_t size, const std::vector<unsigned char> &hash,
                   const std::vector<unsigned char> &sign) : name(name), size(size), hash(hash), sign(sign)
{ }

void Resource::toByteStream(std::vector<unsigned char>& byteArray) const
{
    const char *data = name.data();
    // TODO 4B for name length in documentation, change it to 8 and leave unsigned long as returned by string::length?
    // same with vector::size in other functions
    const int length = name.length() & INT_MAX;
    intToBytes(length, byteArray);
    byteArray.insert(byteArray.end(), data, data + length);
    int64ToBytes(size, byteArray);
    byteArray.insert(byteArray.end(), hash.begin(), hash.end());
    byteArray.insert(byteArray.end(), sign.begin(), sign.end());
}

Resource Resource::fromByteStream(const std::vector<unsigned char>& vector, int& index)
{
    const int length = intFromBytes(vector, index);
    index += 4;
    const std::string name(reinterpret_cast<const char*>(&vector[index]), length);
    index += length;
    const int64_t size = int64FromBytes(vector, index);
    index += 8;
    const std::vector<unsigned char> hash(&vector[index], &vector[index + 16]);
    index += 16;
    const std::vector<unsigned char> sign(&vector[index], &vector[index + 128]);
    index += 128;
    return Resource(std::move(name), size, std::move(hash), std::move(sign));
}

bool operator==(const Resource& left, const Resource& right)
{
    return left.name == right.name && left.size == right.size && right.hash == left.hash && left.sign == right.sign;
}

const std::vector<unsigned char> &Resource::getSign() const
{
    return sign;
}

const std::vector<unsigned char> &Resource::getHash() const
{
    return hash;
}

int64_t Resource::getSize() const
{
    return size;
}

const std::string &Resource::getName() const
{
    return name;
}