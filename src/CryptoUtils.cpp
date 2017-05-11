//
// Created by kamil on 11/05/17.
//

#include <openssl/md5.h>
#include <string>
#include <vector>
#include "CryptoUtils.h"
#include <fstream>

std::vector<unsigned char> hashMd5(std::string string)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)string.c_str(), string.length(), (unsigned char*)&digest);
    return std::vector<unsigned char>(std::begin(digest), std::end(digest));
}

std::string toMd5String(std::vector<unsigned char> digest)
{
    char mdString[33];
    for (int i = 0; i < 16; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    return std::string(mdString);
}

std::vector<unsigned char> hashMd5FromFile(std::string filePath)
{
    std::ifstream fileStream(filePath, std::ifstream::binary);
    if (fileStream)
    {
        unsigned char digest[MD5_DIGEST_LENGTH];
        MD5_CTX mdContext;
        MD5_Init (&mdContext);
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        while (fileStream.read(buffer, BUFFER_SIZE))
            MD5_Update (&mdContext, buffer, BUFFER_SIZE);

        // if the bytes of the block are less than 1024
        MD5_Update (&mdContext, buffer, fileStream.gcount());
        MD5_Final (digest,&mdContext);
        fileStream.close();
        return std::vector<unsigned char>(std::begin(digest), std::end(digest));
    }
    throw std::runtime_error("File " + filePath + " not found, cannot calculate md5 hash.");
}