//
// Created by erxyi on 22.05.17.
//

#ifndef SIMPLE_P2P_GENERICFILEREQUEST_HPP
#define SIMPLE_P2P_GENERICFILEREQUEST_HPP


#include "Files/FileManagerTypes.h"
#include "Files/MD5Utils.h"

struct GenericFileRequest
{
    GenericFileRequest(const AuthorKey &authorKey, const HashArray &fileHash) : authorKey(authorKey),
                                                                                  fileHash(fileHash) {}

    AuthorKey authorKey;
    HashArray fileHash;
};


#endif //SIMPLE_P2P_GENERICFILEREQUEST_HPP
