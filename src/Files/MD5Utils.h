//
// Created by erxyi on 22.05.2017.
//

#ifndef SIMPLE_P2P_MD5UTILS_H
#define SIMPLE_P2P_MD5UTILS_H
#include <array>
#include <string>
#include <openssl/md5.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

typedef std::array<unsigned char, 16> HashArray;

class MD5Utils {
public:
    static HashArray ifstreamToHashArray(std::istream &ifs);
    static HashArray textToHashArray(const std::string &input);
    static const std::string hashArrayToHashASCII(HashArray ha);
    static HashArray boostPathToHashArray(const boost::filesystem::path & p);
};


#endif //SIMPLE_P2P_MD5UTILS_H
