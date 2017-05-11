//
// Created by kamil on 11/05/17.
//

#ifndef SIMPLE_P2P_CRYPTOUTILS_H
#define SIMPLE_P2P_CRYPTOUTILS_H

#include <string>
#include <vector>

std::vector<unsigned char> hashMd5(std::string string);
std::string toMd5String(std::vector<unsigned char> digest);
std::vector<unsigned char> hashMd5FromFile(std::string filePath);

#endif //SIMPLE_P2P_CRYPTOUTILS_H
