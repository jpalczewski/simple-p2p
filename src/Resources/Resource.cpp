//
// Created by kamil on 14/05/17.
//

#include "Resource.h"
#include "../ConversionUtils.h"

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

Resource Resource::fromByteStream(const std::vector<unsigned char>& vector, int index)
{
    throw std::logic_error("Not yet implemented");
}