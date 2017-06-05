//
// Created by erxyi on 31.05.2017.
//

#include "AuthorKey.h"

bool AuthorKey::initialized = false;

AuthorKey::AuthorKey(const std::string &publicKeyFilename, const std::string &privateKeyFilename) : publicKeyFilename(
        publicKeyFilename), privateKeyFilename(privateKeyFilename) {
    if(!initialized) {
        initOpenSSL();
        initialized = true;
    }
}

bool AuthorKey::RSASign(std::string privateKeyFile, const unsigned char *Msg, size_t MsgLen, unsigned char **EncMsg,
                        size_t *MsgLenEnc) {
    EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
    RSA* rsa = NULL;
    if(cachedPrivateKey.empty())
    {
        cachedPrivateKey = loadKey(privateKeyFilename);
    }
    rsa = createPrivateRSA(cachedPrivateKey);
    //EVP_PKEY* priKey = NULL;
    //FILE *keyFile = fopen(privateKeyFile.c_str(), "r");
    EVP_PKEY* priKey  = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(priKey, rsa);

    //priKey = PEM_read_PrivateKey(keyFile,NULL,NULL,NULL);
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

std::vector<unsigned char> AuthorKey::signMessage(const std::string &plainText) {
    unsigned char* encMessage;
    size_t encMessageLength;
    //std::string privateKey = this->loadKey(privateKeyFilename);
    RSASign(privateKeyFilename, (unsigned char*) plainText.c_str(), plainText.length(), &encMessage, &encMessageLength);
    std::vector<unsigned char> signedMessaage(encMessage, encMessage + encMessageLength);
    free(encMessage);
    return signedMessaage;
}

RSA *AuthorKey::createPublicRSA(std::string key) {
    ERR_clear_error();
    RSA *rsa = NULL;
    BIO *keybio;
    const char* c_string = key.c_str();
    keybio = BIO_new_mem_buf((void*)c_string, -1);
    if (keybio==NULL) {
        return 0;
    }
    rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa,NULL, NULL);
    if(rsa == NULL)
    {
        char buffer[120];
        unsigned long val = ERR_get_error();
        ERR_error_string(val, buffer);
        throw std::runtime_error(buffer);
    }
    return rsa;
}

bool AuthorKey::RSAVerifySignature(RSA *rsa, unsigned char *MsgHash, size_t MsgHashLen, const char *Msg, size_t MsgLen,
                                   bool *Authentic) {
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

bool AuthorKey::verifySignature(const std::string &plainText, unsigned char *encryptedMessage,
                                size_t encryptedMessageLength) {
    std::string key;
    if(cachedPublicKey.empty()) {
        key = this->loadKey(publicKeyFilename);
        cachedPublicKey = key;
    }
    else
        key = cachedPublicKey;

    RSA* publicRSA = createPublicRSA(key);
    bool authentic;
    bool result = RSAVerifySignature(publicRSA, encryptedMessage, encryptedMessageLength, plainText.c_str(), plainText.length(), &authentic);
    return result && authentic;
}

RSA *AuthorKey::createPrivateRSA(std::string key) {
    RSA *rsa = NULL;
    const char* c_string = key.c_str();
    BIO * keybio = BIO_new_mem_buf((void*)c_string, -1);
    if (keybio==NULL) {
        return 0;
    }
    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    return rsa;
}

void AuthorKey::generateKey(int bits) {
    int rc;

    RSA_ptr rsa(RSA_new(), ::RSA_free);
    BN_ptr bn(BN_new(), ::BN_free);
#ifdef OPENSSL_SYS_WIN32_CYGWIN
    BIO_FILE_ptr pem1(BIO_new_file(publicKeyFilename.c_str(), "wb"), ::BIO_free);
    BIO_FILE_ptr pem4(BIO_new_file(privateKeyFilename.c_str(), "wb"), ::BIO_free);
#else
    BIO_FILE_ptr pem1(BIO_new_file(publicKeyFilename.c_str(), "w"), ::BIO_free);
    BIO_FILE_ptr pem4(BIO_new_file(privateKeyFilename.c_str(), "w"), ::BIO_free);
#endif
    rc = BN_set_word(bn.get(), RSA_F4);
    if (rc != 1)
        throw std::runtime_error("Public key generation failed on BN_set_word");

    // Generate key
    rc = RSA_generate_key_ex(rsa.get(), bits, bn.get(), NULL);
    if (rc != 1)
        throw std::runtime_error("Public key generation failed on RSA_generate_key_ex");

    // Convert RSA to PKEY
    EVP_KEY_ptr pkey(EVP_PKEY_new(), ::EVP_PKEY_free);
    rc = EVP_PKEY_set1_RSA(pkey.get(), rsa.get());
    if (rc != 1)
        throw std::runtime_error("Public key generation failed on EVP_PKEY");

    // Write public key in PKCS PEM
    rc = PEM_write_bio_RSAPublicKey(pem1.get(), rsa.get());
    if (rc != 1)
        throw std::runtime_error("Public key generation failed on PEM_WRITE_bio");

    // Write private key in PKCS PEM
    rc = PEM_write_bio_RSAPrivateKey(pem4.get(), rsa.get(), NULL, NULL, 0, NULL, NULL);
    if (rc != 1)
        throw std::runtime_error("Public key generation failed on PEM_write_bio");
}

std::string AuthorKey::loadKey(const std::string &filename) {
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Cannot open file " + filename + " to read a key.");
    std::string str;
    std::string fileContents;
    while (std::getline(file, str))
    {
        fileContents += str;
        fileContents.push_back('\n');
    }
    return fileContents;
}

Hash AuthorKey::getPublicPEMHash()  {
   // std::string key = this->loadKey(publicKeyFilename);
    if (!cachedPublicKey.empty())
        return Hash(cachedPublicKey, Hash::InputTextType::Text);
    return Hash(publicKeyFilename, Hash::InputTextType::File);
}

AuthorKey::AuthorKey() {

}

void AuthorKey::loadPublicKeyFromString(const std::string &key) {
    cachedPublicKey = key;
}

void AuthorKey::loadPrivateKeyFromString(const std::string &key) {
    cachedPrivateKey = key;
}

std::string AuthorKey::getPrivateKey() {
    if(cachedPublicKey.empty())
        cachedPrivateKey = loadKey(privateKeyFilename);
    return cachedPrivateKey;
}

std::string AuthorKey::getPublicKey() {
    if(cachedPublicKey.empty())
        cachedPublicKey = loadKey(publicKeyFilename);
    return cachedPublicKey;
}

std::vector<unsigned char> AuthorKey::signMessage(
        const std::vector<unsigned char> &inputData) {
    unsigned char* encMessage;
    size_t encMessageLength;
    RSASign(privateKeyFilename, (unsigned char*) &inputData[0], inputData.size(), &encMessage, &encMessageLength);
    std::vector<unsigned char> signedMessaage(encMessage, encMessage + encMessageLength);
    free(encMessage);
    return signedMessaage;

}
