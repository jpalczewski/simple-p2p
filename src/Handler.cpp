//
// Created by kamil on 05/05/17.
//

#include <iostream>
#include <vector>
#include "Handler.h"
#include "ConversionUtils.h"
#include "Files/FileManager.h"
#include "Messages/SendResourceMessage.h"
#include "Resources/ResourceManager.h"

void Handler::handle(Socket connection)
{
    try
    {
        std::vector<unsigned char> buffer(5);
        int received = connection.read(&buffer[0], 1);
        if (buffer[0] == (char) MessageType::ResourceRequest)
        {
            std::cout << "Received resource request." << std::endl;
            processResourceRequest(std::move(buffer), connection);
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Handling tcp connection failed. Reason: " << e.what() << std::endl;
    }
}

void Handler::processResourceRequest(std::vector<unsigned char> buffer, Socket &connection)
{
    int readTotal = 1; // we already read message type
    while (readTotal < 5) // we need first 5 bytes to read the file name length and know the message size
    {
        int read;
        if ((read = connection.read(&buffer[readTotal], buffer.size() - 1)) < 0)
            throw std::runtime_error("Error during reading resource request from tcp socket.");
        readTotal += read;
    }
    int nameLength = intFromBytes(buffer, 1);
    const int messageSize = 1 + 4 +  nameLength + 8 + 16 + 128 + 8 + 8 + 251;
    auto allBytes = readBytes(connection, std::move(buffer), messageSize);
    ResourceRequestMessage message = ResourceRequestMessage::fromByteStream(std::move(allBytes));
    processResourceRequestMessage(std::move(message), connection);
}

std::vector<unsigned char> Handler::readBytes(Socket& connection, std::vector<unsigned char> buffer, const int size)
{
    std::vector<unsigned char> output(size);
    for(int i = 0; i < 5; ++i)
        output[i] = buffer[i];
    int readTotal = 5; // we already read message type and name length
    while (readTotal < size)
    {
        int read;
        if ((read = connection.read(&output[readTotal], size - readTotal)) < 0)
            throw std::runtime_error("Error during reading resource request from tcp socket.");
        readTotal += read;
    }
    return output;
}

void Handler::processResourceRequestMessage(ResourceRequestMessage message, Socket& connection)
{
    std::cout << "File " << message.getResource().getName() << " requested. " << std::endl;
    LocalResourceInfo info = resourceManager.getLocalOrSharedResourceInfo(message.getPublicKey(), message.getResource());
    if (info.getResourceState() != Resource::State::Active)
    {
        // TODO add error description
        std::vector<unsigned char> errorResponse = {(unsigned char) MessageType::Error};
        std::cout << "File " << message.getResource().getName() << " requested but is in invalid/blocked state. " << std::endl;
        connection.write(errorResponse.data(), 1);
        return;
    }
    FilePartRequest request(message.getPublicKey(), message.getResource().getHash(), message.getOffset(), message.getSize());
    FilePartResponse part = fileManagerInstance.getFilePart(request);
    SendResourceMessage response(message.getResource(), message.getOffset(), message.getSize(), part.received);
    const std::vector<unsigned char>& responseStream = response.toByteStream();
    std::cout << "Sending file " << message.getResource().getName() << " to other node. " << std::endl;
    connection.write(&responseStream[0], responseStream.size());
}





