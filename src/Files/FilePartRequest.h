//
// Created by erxyi on 22.05.2017.
//

#ifndef SIMPLE_P2P_FILEPARTREQUEST_H
#define SIMPLE_P2P_FILEPARTREQUEST_H

#include "FileManagerTypes.h"
#include "GenericFileRequest.h"

struct FilePartRequest : public GenericFileRequest {
    FilePartRequest(const AUTHOR_KEY &authorKey,
                    const HASH_ARRAY &fileHash,
                    std::size_t offset_,
                    std::size_t size_) : GenericFileRequest(authorKey,
                                                           fileHash),
                                        offset(offset_),
                                        size(size_) {}

    std::size_t offset;
    std::size_t size;
};


#endif //SIMPLE_P2P_FILEPARTREQUEST_H
