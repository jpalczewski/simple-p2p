//
// Created by kamil on 20/05/17.
//

#ifndef SIMPLE_P2P_CONVERSIONUTILS_H
#define SIMPLE_P2P_CONVERSIONUTILS_H

#include <cstdint>

// TODO maybe some better place for these functions?

void int64ToBytes(int64_t value, std::vector<unsigned char>& result);
int64_t int64FromBytes(const std::vector<unsigned char>& byteArray, int index);
void intToBytes(int value, std::vector<unsigned char>& result);
int intFromBytes(const std::vector<unsigned char>& byteArray, int index);

#endif //SIMPLE_P2P_CONVERSIONUTILS_H
