//
// Created by erxyi on 22.05.17.
//

#ifndef SIMPLE_P2P_GENERICFILEREQUEST_HPP
#define SIMPLE_P2P_GENERICFILEREQUEST_HPP


#include "../FileManagerTypes.h"
#include "../MD5Utils.h"
#include "../../Crypto/Hash.h"

struct GenericFileRequest
{
    GenericFileRequest(const AuthorKeyType &authorKey, const Hash &fileHash) : authorKey(authorKey),
                                                                           fileHash(fileHash) {}

    AuthorKeyType authorKey;
    Hash fileHash;
};


#endif //SIMPLE_P2P_GENERICFILEREQUEST_HPP
