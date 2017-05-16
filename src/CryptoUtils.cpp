//
// Created by kamil on 11/05/17.
//

#include <openssl/md5.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#include <string>
#include <vector>
#include "CryptoUtils.h"
#include <fstream>
#include <memory>

void initOpenSsl()
{
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
    ERR_load_crypto_strings();
}

std::string readKey(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file)
        throw std::runtime_error("Cannot open file " + fileName + " to read a key.");
    std::string str;
    std::string fileContents;
    while (std::getline(file, str))
    {
        fileContents += str;
        fileContents.push_back('\n');
    }
    return fileContents;
}
std::vector<unsigned char> hashMd5(const std::string& string)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)string.c_str(), string.length(), (unsigned char*)&digest);
    return std::vector<unsigned char>(std::begin(digest), std::end(digest));
}

std::string toMd5String(const std::vector<unsigned char>& digest)
{
    char mdString[33];
    for (int i = 0; i < 16; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    return std::string(mdString);
}

std::vector<unsigned char> hashMd5FromFile(const std::string& filePath)
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

using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
using EVP_KEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using BIO_FILE_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;


void generateKey(const std::string& publicKeyFile, const std::string& privateKeyFile, int bits)
{
    int rc;

    RSA_ptr rsa(RSA_new(), ::RSA_free);
    BN_ptr bn(BN_new(), ::BN_free);

    BIO_FILE_ptr pem1(BIO_new_file(publicKeyFile.c_str(), "w"), ::BIO_free);
    BIO_FILE_ptr pem4(BIO_new_file(privateKeyFile.c_str(), "w"), ::BIO_free);

    rc = BN_set_word(bn.get(), RSA_F4);
    if (rc != 1)
        throw std::runtime_error("Public key generation failed");

    // Generate key
    rc = RSA_generate_key_ex(rsa.get(), bits, bn.get(), NULL);
    if (rc != 1)
        throw std::runtime_error("Public key generation failed");

    // Convert RSA to PKEY
    EVP_KEY_ptr pkey(EVP_PKEY_new(), ::EVP_PKEY_free);
    rc = EVP_PKEY_set1_RSA(pkey.get(), rsa.get());
    if (rc != 1)
        throw std::runtime_error("Public key generation failed");

    // Write public key in PKCS PEM
    rc = PEM_write_bio_RSAPublicKey(pem1.get(), rsa.get());
    if (rc != 1)
        throw std::runtime_error("Public key generation failed");

    // Write private key in PKCS PEM
    rc = PEM_write_bio_RSAPrivateKey(pem4.get(), rsa.get(), NULL, NULL, 0, NULL, NULL);
    if (rc != 1)
        throw std::runtime_error("Public key generation failed");
}

////////////////////////////////////////////
// SIGNING
////////////////////////////////////////////

RSA* createPrivateRSA(std::string key)
{
    RSA *rsa = NULL;
    const char* c_string = key.c_str();
    BIO * keybio = BIO_new_mem_buf((void*)c_string, -1);
    if (keybio==NULL) {
        return 0;
    }
    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    return rsa;
}

bool RSASign( std::string privateKeyFile, const unsigned char* Msg, size_t MsgLen,
              unsigned char** EncMsg, size_t* MsgLenEnc)
{
    EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
    EVP_PKEY* priKey = NULL;
    FILE *keyFile = fopen(privateKeyFile.c_str(), "r");

    priKey = PEM_read_PrivateKey(keyFile,NULL,NULL,NULL);
    if (EVP_DigestSignInit(m_RSASignCtx,NULL, EVP_md5(), NULL,priKey)<=0) {
        return false;
    }
    if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0) {
        return false;
    }
    if (EVP_DigestSignFinal(m_RSASignCtx, NULL, MsgLenEnc) <=0) {
        return false;
    }
    *EncMsg = (unsigned char*)malloc(*MsgLenEnc);
    if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0) {
        return false;
    }
    EVP_MD_CTX_cleanup(m_RSASignCtx);
    return true;
}

std::vector<unsigned char> signMessage(const std::string& privateKeyFile, const std::string& plainText)
{
    unsigned char* encMessage;
    size_t encMessageLength;
    RSASign(privateKeyFile, (unsigned char*) plainText.c_str(), plainText.length(), &encMessage, &encMessageLength);
    std::vector<unsigned char> signedMessaage(encMessage, encMessage + encMessageLength);
    free(encMessage);
    return signedMessaage;
}

////////////////////////////////////////////
// VERIFYING
////////////////////////////////////////////

RSA* createPublicRSA(const std::string& key)
{
    RSA *rsa = NULL;
    BIO *keybio;
    const char* c_string = key.c_str();
    keybio = BIO_new_mem_buf((void*)c_string, -1);
    if (keybio==NULL) {
        return 0;
    }
    rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa,NULL, NULL);
    return rsa;
}

bool RSAVerifySignature( RSA* rsa, unsigned char* MsgHash, size_t MsgHashLen,
                         const char* Msg, size_t MsgLen, bool* Authentic)
{
    *Authentic = false;
    EVP_PKEY* pubKey  = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pubKey, rsa);
    EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_create();

    if (EVP_DigestVerifyInit(m_RSAVerifyCtx,NULL, EVP_md5(),NULL,pubKey)<=0) {
        return false;
    }
    if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, Msg, MsgLen) <= 0) {
        return false;
    }
    int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, MsgHash, MsgHashLen);
    if (AuthStatus==1) {
        *Authentic = true;
        EVP_MD_CTX_cleanup(m_RSAVerifyCtx);
        return true;
    } else if(AuthStatus==0){
        *Authentic = false;
        EVP_MD_CTX_cleanup(m_RSAVerifyCtx);
        return true;
    } else{
        *Authentic = false;
        EVP_MD_CTX_cleanup(m_RSAVerifyCtx);
        return false;
    }
}

bool verifySignature(const std::string& publicKey, const std::string& plainText, unsigned char* encMessage, size_t encMessageLength)
{
    RSA* publicRSA = createPublicRSA(publicKey);
    bool authentic;
    bool result = RSAVerifySignature(publicRSA, encMessage, encMessageLength, plainText.c_str(), plainText.length(), &authentic);
    return result && authentic;
}