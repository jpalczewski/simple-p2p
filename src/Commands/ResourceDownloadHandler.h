//
// Created by kamil on 02/06/17.
//

#ifndef SIMPLE_P2P_RESOURCEDOWNLOADHANDLER_H
#define SIMPLE_P2P_RESOURCEDOWNLOADHANDLER_H


#include "../Resources/Resource.h"
#include "../Network/Socket.h"
#include "../Messages/MessageType.h"

class ResourceDownloadHandler
{
public:
    ResourceDownloadHandler(int targetPort) : targetPort(targetPort) { }

    void downloadResource(std::pair<std::string, Resource> keyResource);

    std::vector<unsigned char> getWholeMessage(Socket& socket, size_t size, MessageType type);

private:
    int targetPort;
};


#endif //SIMPLE_P2P_RESOURCEDOWNLOADHANDLER_H
