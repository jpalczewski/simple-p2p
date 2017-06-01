//
// Created by erxyi on 27.05.2017.
//

#ifndef SIMPLE_P2P_ADDFILEREQUEST_H
#define SIMPLE_P2P_ADDFILEREQUEST_H


#include "GenericFileRequest.h"

struct AddFileRequest : public GenericFileRequest {
    std::string path;
    std::string privateKey;
    AddFileRequest(const AuthorKeyType &publicKey,const AuthorKeyType &privateKey_, std::string path_)
            : GenericFileRequest(publicKey, Hash(path_, Hash::InputTextType::File)),
              path(path_),
              privateKey(privateKey_) {}



};


#endif //SIMPLE_P2P_ADDFILEREQUEST_H
