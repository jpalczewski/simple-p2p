//
// Created by erxyi on 22.05.2017.
//

#ifndef SIMPLE_P2P_FILEPARTREQUEST_H
#define SIMPLE_P2P_FILEPARTREQUEST_H

#include "FileManagerTypes.h"
#include "GenericFileRequest.h"

struct FilePartRequest : public GenericFileRequest {
    FilePartRequest(const AuthorKey &authorKey,
                    const HashArray &fileHash,
                    std::size_t offset_,
                    std::size_t size_,
                    bool allowInvalid_ = false) : GenericFileRequest(authorKey,
                                                           fileHash),
                                        offset(offset_),
                                        size(size_),
                                        allowInvalid(allowInvalid_){}

    std::size_t offset;
    std::size_t size;
    bool        allowInvalid;
};


#endif //SIMPLE_P2P_FILEPARTREQUEST_H
