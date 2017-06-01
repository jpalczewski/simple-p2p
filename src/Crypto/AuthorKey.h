//
// Created by erxyi on 31.05.2017.
//

#ifndef SIMPLE_P2P_AUTHORKEY_H
#define SIMPLE_P2P_AUTHORKEY_H

#include <string>
#include "Hash.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <memory>
#include <vector>
#include <openssl/err.h>
#include <fstream>

using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
using EVP_KEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using BIO_FILE_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;

class AuthorKey {
public:
    AuthorKey(const std::string &publicKeyFilename, const std::string &privateKeyFilename);

    bool verifySignature(const std::string &plainText, unsigned char* encryptedMessage,
                         size_t encryptedMessageLength);
    std::vector<unsigned char> signMessage(const std::string &plainText);
    void generateKey(int bits);
    Hash getPublicPEMHash();
private:
    std::string publicKeyFilename;
    std::string privateKeyFilename;

    static bool initialized;

    static void initOpenSSL()
    {
        OpenSSL_add_all_algorithms();
        OpenSSL_add_all_ciphers();
        ERR_load_crypto_strings();
    }

    bool RSASign( std::string privateKeyFile, const unsigned char* Msg, size_t MsgLen,
                  unsigned char** EncMsg, size_t* MsgLenEnc);
    RSA* createPublicRSA(std::string key);
    RSA* createPrivateRSA(std::string key);
    bool RSAVerifySignature( RSA* rsa, unsigned char* MsgHash, size_t MsgHashLen,
                             const char* Msg, size_t MsgLen, bool* Authentic);

    std::string loadKey(const std::string &filename);
};


#endif //SIMPLE_P2P_AUTHORKEY_H
