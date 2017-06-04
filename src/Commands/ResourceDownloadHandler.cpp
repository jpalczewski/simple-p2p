//
// Created by kamil on 02/06/17.
//

#include "ResourceDownloadHandler.h"
#include "../Resources/ResourceManager.h"
#include "../Files/FileManagerTypes.h"
#include "../Network/Socket.h"
#include "../Messages/ResourceRequestMessage.h"
#include "../Messages/SendResourceMessage.h"
#include "../Files/FileManager.h"
#include <iostream>

void ResourceDownloadHandler::downloadResource(std::pair<AuthorKeyType, Resource> keyResource)
{
    AuthorKeyType &key = keyResource.first;
    Resource &resource = keyResource.second;
    auto info = resourceManager.getNetworkResourceInfo(key, resource);
    auto seeder = *info.getSeeders().begin();
    Socket socket(Socket::Domain::Ip4, Socket::Type::Tcp);
    socket.bind(0);
    socket.connectByIp(seeder.getAddress(), targetPort);
    ResourceRequestMessage message(key, resource, 0, resource.getSize());
    std::vector<unsigned char> byteStream = message.toByteStream();
    std:: cout << "Sending file request for " << resource.getName() << std::endl;
    socket.write(&byteStream[0], byteStream.size());
    std:: cout << "Sent request for " << resource.getName() << std::endl;
    size_t incomingMessageSize = 1 + 4 + resource.getName().length() + 8 + 16 + 128 + 8 + 8 + resource.getSize();
    std::vector<unsigned char> bytes = getWholeMessage(socket, incomingMessageSize);
    SendResourceMessage contentMessage = SendResourceMessage::fromByteStream(std::move(bytes));
    std:: cout << "successfully completed content message for " << resource.getName() << std::endl;
    FileCreateRequest request(key, (const HashArray &) resource.getHash(), resource.getName(), resource.getSize());
    fileManagerInstance.createFile(std::move(request));
    FileSavePartRequest saveRequest(key, (const HashArray &) resource.getHash(), 0, resource.getSize());
    saveRequest.bytes = contentMessage.getData();
    fileManagerInstance.saveFilePart(std::move(saveRequest));
    // TODO add this resource to local resource in resource manager
    std::cout << "Completed downloading resource " << contentMessage.getResource().getName() << std::endl;
}

std::vector<unsigned char> ResourceDownloadHandler::getWholeMessage(Socket& socket, size_t size)
{
    std::vector<unsigned char> output(size);
    int readTotal = 0;
    while (readTotal < size)
    {
        int read;
        if ((read = socket.read(&output[readTotal], size - readTotal)) < 0)
            throw std::runtime_error("Error during reading resource message from tcp socket.");
        readTotal += read;
    }
    return output;
}



