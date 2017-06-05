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
    unsigned char type;
    socket.read(&type, 1);
    if (type != (unsigned char) MessageType::SendResource)
        throw std::runtime_error("Downloading resource failed.");
    size_t incomingMessageSize = 1 + 4 + resource.getName().length() + 8 + 16 + 128 + 8 + 8 + resource.getSize();
    std::vector<unsigned char> bytes = getWholeMessage(socket, incomingMessageSize, MessageType::SendResource);
    SendResourceMessage contentMessage = SendResourceMessage::fromByteStream(std::move(bytes));
    std:: cout << "Successfully completed content message for " << resource.getName() << std::endl;
    FileCreateRequest request(key, (const HashArray &) resource.getHash(), resource.getName(), resource.getSize());
    fileManagerInstance.createFile(std::move(request));
    FileSavePartRequest saveRequest(key, (const HashArray &) resource.getHash(), 0, resource.getSize());
    saveRequest.bytes = contentMessage.getData();
    fileManagerInstance.saveFilePart(std::move(saveRequest));
    // TODO broadcast this resource
    resourceManager.addLocalResource(message.getPublicKey(), message.getResource());
    std::cout << "Completed downloading resource " << contentMessage.getResource().getName() << std::endl;
}

std::vector<unsigned char> ResourceDownloadHandler::getWholeMessage(Socket& socket, size_t size, MessageType type)
{
    std::vector<unsigned char> output(size);
    output[0] = static_cast<unsigned char>(type);
    int readTotal = 1;
    while (readTotal < size)
    {
        int read;
        if ((read = socket.read(&output[readTotal], size - readTotal)) < 0)
            throw std::runtime_error("Error during reading resource message from tcp socket.");
        readTotal += read;
    }
    return output;
}



