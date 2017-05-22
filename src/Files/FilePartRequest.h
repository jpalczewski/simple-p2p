//
// Created by erxyi on 22.05.2017.
//

#ifndef SIMPLE_P2P_FILEPARTREQUEST_H
#define SIMPLE_P2P_FILEPARTREQUEST_H

#include "FileManagerTypes.h"

struct FilePartRequest {
    AUTHOR_KEY authorKey;
    HASH_ARRAY fileHash;
    std::size_t offset;
    std::size_t size;
};


#endif //SIMPLE_P2P_FILEPARTREQUEST_H
