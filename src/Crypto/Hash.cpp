//
// Created by erxyi on 31.05.2017.
//

#include <iomanip>
#include "Hash.h"



Hash::Hash(const std::array<unsigned char, 16> &hash) : hash(hash) {}

std::string Hash::getString() const {
    std::ostringstream ostringstream;
    ostringstream << std::hex << std::setfill('0');

    for(auto &i : hash)
        ostringstream << std::setw(2) <<  static_cast<unsigned>(i);

    return ostringstream.str();
}

Hash::Hash(const std::string &inputText, InputTextType inputType) {
    if(inputType == InputTextType::Text)
        MD5((unsigned char*)inputText.c_str(), inputText.length(), hash.data());
    else if(inputType == InputTextType::Hash)
        throw std::logic_error("Not implemented yet"); //TODO: https://stackoverflow.com/questions/17261798/converting-a-hex-string-to-a-byte-array
}

Hash::Hash(std::istream &inputStream) {
    getHashFromInputStream(inputStream);

}

void Hash::getHashFromInputStream(std::istream &inputStream)  {
    std::array<char, BUFFER_SIZE> readBuffer;
    MD5_CTX CTX;

    MD5_Init(&CTX);

    inputStream.seekg(0, std::ios_base::beg);

    if(!inputStream.good())
        throw std::runtime_error("Hash::Hash received bad stream");

    while(!inputStream.eof())
    {
        inputStream.read(readBuffer.data(), BUFFER_SIZE);
        MD5_Update(&CTX, readBuffer.data(), (size_t) inputStream.gcount());
    }

    MD5_Final(hash.data(), &CTX);
}

Hash::Hash(boost::filesystem::path &p) {
    using namespace boost::filesystem;
    ifstream ifs{p, std::ios::binary};
    getHashFromInputStream(ifs);
    ifs.close();
}

std::array<unsigned char, 16> Hash::getArray() const {
    return hash;
}
