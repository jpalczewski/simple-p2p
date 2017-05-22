//
// Created by erxyi on 22.05.2017.
//

#include <sstream>
#include <iomanip>
#include "MD5Utils.h"

const int buffer_size = 4096;

HASH_ARRAY MD5Utils::textToHashArray(const std::string &input) {
    HASH_ARRAY md5;
    MD5((unsigned char*)input.c_str(), input.length(), md5.data());
    return md5;
}

const std::string MD5Utils::hashArrayToHashASCII(HASH_ARRAY ha) {
    std::ostringstream ostringstream;
    ostringstream << std::hex << std::setfill('0');

    for(auto &i : ha)
        ostringstream << std::setw(2) <<  static_cast<unsigned>(i);

    return ostringstream.str();
}

HASH_ARRAY MD5Utils::ifstreamToHashArray(std::istream &ifs) {
    HASH_ARRAY md5;
    std::array<char, buffer_size> readBuffer;

    MD5_CTX CTX;
    MD5_Init(&CTX);

    ifs.seekg(0, std::ios::beg);

    if(!ifs.good())
        throw std::runtime_error("ifstreamToHashArray received bad stream");

    while(!ifs.eof())
    {
        ifs.read(readBuffer.data(), buffer_size);
        MD5_Update(&CTX, readBuffer.data(), (size_t) ifs.gcount());
    }

    MD5_Final(md5.data(), &CTX);
    return md5;
}

HASH_ARRAY MD5Utils::boostPathToHashArray(const boost::filesystem::path &p) {
    using namespace boost::filesystem;
    std::ifstream filestream = ifstream{p, std::ios::binary};
    HASH_ARRAY md5fromCWD = MD5Utils::ifstreamToHashArray(filestream);
    filestream.close();
    return md5fromCWD;
}
