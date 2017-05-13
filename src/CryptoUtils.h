//
// Created by kamil on 11/05/17.
//

#ifndef SIMPLE_P2P_CRYPTOUTILS_H
#define SIMPLE_P2P_CRYPTOUTILS_H

#include <string>
#include <vector>

// read public or private key from file as string
std::string readKey(const std::string& fileName);

void initOpenSsl();
void generateKey(const std::string& publicKeyFile, const std::string& privateKeyFile, int bits = 1024);

// hashing
std::vector<unsigned char> hashMd5(const std::string& string);
std::string toMd5String(const std::vector<unsigned char>& digest);
std::vector<unsigned char> hashMd5FromFile(const std::string& filePath);

// signing and verifying
bool verifySignature(const std::string& publicKey, const std::string& plainText, unsigned char* encMessage, size_t encMessageLength);
std::vector<unsigned char> signMessage(const std::string& privateKeyFile, const std::string& plainText);

#endif //SIMPLE_P2P_CRYPTOUTILS_H
