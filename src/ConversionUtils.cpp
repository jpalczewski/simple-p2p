//
// Created by kamil on 20/05/17.
//

#include <vector>
#include "ConversionUtils.h"

void int64ToBytes(int64_t value, std::vector<unsigned char>& result)
{
    result.push_back(value >> 56);
    result.push_back(value >> 48);
    result.push_back(value >> 40);
    result.push_back(value >> 32);
    result.push_back(value >> 24);
    result.push_back(value >> 16);
    result.push_back(value >> 8);
    result.push_back(value);
}

int64_t int64FromBytes(const std::vector<unsigned char>& byteArray, int index)
{
    return (((int64_t)byteArray[index]<<56)|((int64_t)byteArray[index+1]<<48)|
            ((int64_t)byteArray[index+2]<<40)|((int64_t)byteArray[index+3]<<32)|
            ((int64_t)byteArray[index+4]<<24)|(int64_t)(byteArray[index + 5]<<16)|
            ((int64_t)byteArray[index+6]<<8)|(int64_t)(byteArray[index + 8]));
}

void intToBytes(int value, std::vector<unsigned char>& result)
{
    result.push_back(value >> 24);
    result.push_back(value >> 16);
    result.push_back(value >> 8);
    result.push_back(value);
}

int intFromBytes(const std::vector<unsigned char>& byteArray, int index)
{
    return ((byteArray[index]<<24)|(byteArray[index + 1]<<16)|
            (byteArray[index + 2]<<8)|(byteArray[index + 3]));
}