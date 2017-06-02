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

void ResourceDownloadHandler::downloadResource(std::pair<AuthorKeyType, Resource>resource)
{
    auto info = resourceManager.getNetworkResourceInfo(resource.first, resource.second);
    auto seeder = *info.getSeeders().begin();
    Socket socket(Socket::Domain::Ip4, Socket::Type::Tcp);
    socket.bind(0);
    socket.connectByIp(seeder.getAddress(), 5000);
    ResourceRequestMessage message(resource.first, resource.second, 0, resource.second.getSize());
    std::vector<unsigned char> byteStream = message.toByteStream();
    std:: cout << "Sending file request for " << resource.second.getName() << std::endl;
    socket.write(&byteStream[0], byteStream.size());
    std:: cout << "Received data for " << resource.second.getName() << std::endl;
    size_t incomingMessageSize = 1 + 8 + 8 + 16 + 128 + 8 + resource.second.getName().length() + resource.second.getSize();
    std::vector<unsigned char> bytes = getWholeMessage(socket, incomingMessageSize);
    SendResourceMessage contentMessage = SendResourceMessage::fromByteStream(std::move(bytes));
    FileCreateRequest request(resource.first, (const HashArray &) resource.second.getHash(), resource.second.getName(), resource.second.getSize());
    fileManagerInstance.createFile(std::move(request));
    FileSavePartRequest saveRequest(resource.first, (const HashArray &) resource.second.getHash(), 0, resource.second.getSize());
    saveRequest.bytes = contentMessage.getData();
    fileManagerInstance.saveFilePart(std::move(saveRequest));
    std::cout << "Completed downloading resource " << contentMessage.getResource().getName() << std::endl;
}

std::vector<unsigned char> ResourceDownloadHandler::getWholeMessage(Socket& socket, size_t size)
{
    std::vector<unsigned char> bytes;
    bytes.reserve(size);
    while (bytes.size() < size)
    {
        socket.read(&bytes[bytes.size()], size - bytes.size());
    }
    return bytes;
}



