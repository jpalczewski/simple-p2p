//
// Created by erxyi on 27.05.2017.
//

#ifndef SIMPLE_P2P_ADDFILEREQUEST_H
#define SIMPLE_P2P_ADDFILEREQUEST_H


#include "GenericFileRequest.h"

struct AddFileRequest : public GenericFileRequest {
    std::string path;

    AddFileRequest(const AuthorKey &authorKey, std::string path_) : GenericFileRequest(authorKey, Hash(path_, Hash::InputTextType::File)), path(path_) {

    }

};


#endif //SIMPLE_P2P_ADDFILEREQUEST_H
