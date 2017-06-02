//
// Created by kamil on 02/06/17.
//

#ifndef SIMPLE_P2P_RESOURCEDOWNLOADHANDLER_H
#define SIMPLE_P2P_RESOURCEDOWNLOADHANDLER_H


#include "../Resources/Resource.h"
#include "../Network/Socket.h"

class ResourceDownloadHandler
{
public:
    void downloadResource(std::pair<std::string, Resource> resource);

    std::vector<unsigned char> getWholeMessage(Socket& socket, size_t size);
};


#endif //SIMPLE_P2P_RESOURCEDOWNLOADHANDLER_H
