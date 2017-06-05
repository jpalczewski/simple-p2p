//
// Created by erxyi on 05.06.17.
//

#ifndef SIMPLE_P2P_FILESTATEREQUEST_H
#define SIMPLE_P2P_FILESTATEREQUEST_H



#include "GenericFileRequest.h"

struct SetFileStateRequest : GenericFileRequest {
    FileRecordState state;

    SetFileStateRequest(const AuthorKeyType &authorKey,
                        const Hash &fileHash,
                        const FileRecordState state_) :
            GenericFileRequest(authorKey,
                               fileHash),
            state(state_) {}

};


#endif //SIMPLE_P2P_FILESTATEREQUEST_H
