//
// Created by erxyi on 22.05.17.
//

#ifndef SIMPLE_P2P_GENERICFILEREQUEST_HPP
#define SIMPLE_P2P_GENERICFILEREQUEST_HPP


#include "FileManagerTypes.h"
#include "MD5Utils.h"

struct GenericFileRequest
{
    GenericFileRequest(const AuthorKey &authorKey, const HASH_ARRAY &fileHash) : authorKey(authorKey),
                                                                                  fileHash(fileHash) {}

    AuthorKey authorKey;
    HASH_ARRAY fileHash;
};


#endif //SIMPLE_P2P_GENERICFILEREQUEST_HPP
