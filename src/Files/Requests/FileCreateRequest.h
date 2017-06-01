//
// Created by erxyi on 22.05.17.
//

#ifndef SIMPLE_P2P_FILECREATEREQUEST_HPP
#define SIMPLE_P2P_FILECREATEREQUEST_HPP


#include "GenericFileRequest.h"

struct FileCreateRequest : public GenericFileRequest
{
    FileCreateRequest(const AuthorKeyType &authorKey, const HashArray &fileHash, std::string & name_, std::size_t length_)
            : GenericFileRequest(authorKey,
                                 fileHash),
            name(name_),
            length(length_){}

    std::string name;
    std::size_t length;


};


#endif //SIMPLE_P2P_FILECREATEREQUEST_HPP
