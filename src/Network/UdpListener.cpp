//
// Created by kamil on 18/05/17.
//

#include <iostream>
#include "UdpListener.h"
#include "../Resources/ResourceManager.h"
#include "../Messages/BroadcastMessage.h"
#include "../Messages/ResourceRequestMessage.h"
#include "../Messages/ResourceManagementMessage.h"
#include "../Crypto/AuthorKey.h"
#include "../Files/FileManager.h"
#include "../Files/Requests/SetFileStateRequest.h"

UdpListener::UdpListener(int listenPort) : socket(Socket::Domain::Ip4, Socket::Type::Udp)
{
    socket.bind(listenPort);
}

void UdpListener::start()
{
    std::vector<unsigned char> buffer(bufferSize);
    while (true)
    {
        // TODO make it multithreaded and remember sender ip in resource info
        // and handle messages bigger than buffer
        // and refactor it, generally
        IpAddress sender;
        int received = socket.readFrom(&buffer[0], bufferSize, sender);
        std::cout << "Received broadcast from " + sender.toString() << std::endl;
        if (received == bufferSize)
            throw std::runtime_error("buffer too small");
//        std::cout << std::to_string(received) << " bytes received" << std::endl;
        
        switch (static_cast<MessageType >(buffer[0]))
        {
            case MessageType::BroadcastResource:
                handleBroadcastMessage(buffer, sender);
                break;
            case MessageType::BlockResource:
            case MessageType::DeleteResource:
            case MessageType::InvalidateResource:
            case MessageType::UnblockResource:
                handleResourceManagementMessage(buffer, static_cast<MessageType>(buffer[0]));
        }
    }
}

void UdpListener::handleResourceManagementMessage(std::vector<unsigned char> buffer, MessageType type) const
{
    ResourceManagementMessage message = ResourceManagementMessage::fromByteStream(move(buffer), 1);
    if (!verifySignature(message, type))
    {
        std::cout << "Received unauthorized resource state change request for " << message.getResource().getName() << std::endl;
        return;
    }
    if (type == MessageType::DeleteResource)
    {
        handleDelete(std::move(message));
        return;
    }
    const auto found = StateForMessage.find(type);
    if (found == StateForMessage.end())
    {
        std::cout << "Unknown resource state change request received." << std::endl;
        return;
    }
    resourceManager.setNetworkResourceInfoState(message.getPublicKey(), message.getResource(), found->second);
    resourceManager.trySetLocalResourceInfoState(message.getPublicKey(), message.getResource(), found->second);
    std::cout << "Resource state change request completed: " << message.getResource().getName() << std::endl;
}

void UdpListener::handleBroadcastMessage(std::vector<unsigned char> buffer, const IpAddress &sender) const
{
    BroadcastMessage message = BroadcastMessage::fromByteStream(move(buffer));
    for (const auto& keyResource : message.getResources())
    {
        for (const auto &resource : keyResource.second)
        {
            const std::vector<IpAddress> seeders = {sender};
            resourceManager.addNetworkResource(keyResource.first, resource, seeders);
        }
    }
}

void UdpListener::handleDelete(ResourceManagementMessage message) const
{
    std::cout << "handleDelete in action" << std::endl;
    SetFileStateRequest sfss(message.getPublicKey(), Hash(message.getResource().getHash()), FileRecordState::Deleted);
    fileManagerInstance.setFileState(sfss);
}

bool UdpListener::verifySignature(ResourceManagementMessage &message, MessageType type) const
{
    AuthorKey key;
    key.loadPublicKeyFromString(message.getPublicKey());
    std::stringstream plainText;
    plainText << (char)type;
    const std::vector<unsigned char>&vector = message.toByteStream(false);
    plainText << std::string(reinterpret_cast<const char*>(vector.data()), vector.size());
    bool verified = key.verifySignature(plainText.str(), message.getSign().data(), 128);
    return verified;
}







