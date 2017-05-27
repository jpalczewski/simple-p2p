//
// Created by kamil on 05/05/17.
//

#include <iostream>
#include <vector>
#include "Handler.h"
#include "ConversionUtils.h"

void Handler::handle(Socket connection)
{
    std::vector<unsigned char> buffer(1024);
    int received = connection.read(&buffer[0], 1);
    if (buffer[0] == (char) MessageType::ResourceRequest)
    {
        std::cout << "Received resource request." << std::endl;
        proceedResourceRequest(std::move(buffer), connection);
    }
}

void Handler::proceedResourceRequest(std::vector<unsigned char> buffer, Socket& connection)
{
    while (buffer.size() < 5) // we need first 5 bytes to read the file name length and know the message size
    {
        if (connection.read(&buffer[1], buffer.capacity())< 0)
            throw std::runtime_error("Error during reading resource request from tcp socket.");
    }
    int nameLength = intFromBytes(buffer, 1);
    const int messageSize = 1 + nameLength + + 8 + 16 + 128 + 8 + 8;
    readBytes(connection, buffer, messageSize);
    ResourceRequestMessage message = ResourceRequestMessage::fromByteStream(std::move(buffer));
    proceedResourceRequestMessage(std::move(message));
}

void Handler::readBytes(Socket& connection, std::vector<unsigned char>& buffer, const int size)
{
    if (buffer.capacity() < size)
        buffer.reserve(size);
    while (buffer.size() < size)
    {
        if (connection.read(&buffer[buffer.size()], buffer.capacity() < 0))
            throw std::runtime_error("Error during reading message from tcp socket.");
    }
}

void Handler::proceedResourceRequestMessage(ResourceRequestMessage message)
{
    std::cout << "File " << message.getResource().getName() << "requested. " << std::endl;
}





