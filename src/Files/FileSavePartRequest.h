//
// Created by erxyi on 22.05.17.
//

#ifndef SIMPLE_P2P_FILESAVEPARTREQUEST_HPP
#define SIMPLE_P2P_FILESAVEPARTREQUEST_HPP
#include "FilePartRequest.h"

struct FileSavePartRequest : public FilePartRequest
{
    FileSavePartRequest(const AuthorKey &authorKey, const HASH_ARRAY &fileHash, size_t offset_, size_t size_)
            : FilePartRequest(authorKey, fileHash, offset_, size_), bytes() {bytes.resize(size_, 0xDE);}

    std::vector<char> bytes;
};


#endif //SIMPLE_P2P_FILESAVEPARTREQUEST_HPP
