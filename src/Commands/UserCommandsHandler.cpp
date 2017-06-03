//
// Created by kamil on 25/05/17.
//

#include "UserCommandsHandler.h"
#include "../Messages/BroadcastMessage.h"
#include "AddCommand.h"
#include "DownloadCommand.h"
#include "NetworkCommandInterface.h"
#include "ResourceDownloadHandler.h"
#include "../Files/FileManager.h"

namespace
{
    template<typename InfoType>
    void printResourceMap(const ResourceManager::ResourceMap<InfoType> &resources, std::ostream& stream)
    {
        for (const auto &keyRes : resources)
        {
            stream << "key: " << std::endl;
            stream << keyRes.first << std::endl;
            for (const auto &res : keyRes.second)
            {
                stream << "Id: " << res.second.getLocalId() << std::endl;
                stream << res.first.toString() << std::endl;
                stream << res.second.toString() << std::endl;
            }
        }
    }
}

UserCommandsHandler::UserCommandsHandler(int port, int clientPort) : broadcastPort(port), socket(Socket::Domain::Ip4, Socket::Type::Udp),
                                                     commandInterface(std::make_unique<NetworkCommandInterface>(clientPort)),
                                                                     downloader(port)
{
    socket.enableBroadcast();
}

UserCommandsHandler::~UserCommandsHandler()
{
    socket.close();
}

void UserCommandsHandler::handleUserInput()
{
    while (true)
    {
        std::unique_ptr<Command> command = commandInterface->getNextCommand();
        command->accept(this);
    }
}

std::pair<AuthorKeyType, Resource> UserCommandsHandler::resourceFromFile(std::string fileName)
{
    const std::string publicKeyFileName = "/home/kamil/Projects/simple-p2p/src/rsa_public.pem";
    const std::string privateKeyFileName = "/home/kamil/Projects/simple-p2p/src/rsa_private.pem";
    AuthorKey key(publicKeyFileName, privateKeyFileName);
    AddFileRequest request(key.getPublicKey(), key.getPrivateKey(), fileName);
    auto hashAndSign = fileManagerInstance.addFile(request);
    size_t size = file_size(fileName);
    return std::make_pair(key.getPublicKey(), Resource(fileName, size, hashAndSign.first.getVector(), hashAndSign.second));
}

std::unordered_map<std::string, std::vector<Resource>> UserCommandsHandler::convertInfoMapToResourceMap(ResourceManager::ResourceMap<LocalResourceInfo> map)
{
    std::unordered_map<std::string, std::vector<Resource>> allResources;
    for (const auto& keyResource : map)
    {
        std::vector<Resource> resources;
        for (const auto& resourceWithInfo : keyResource.second)
        {
            resources.push_back(resourceWithInfo.first);
        }
        allResources.insert(std::make_pair(keyResource.first, std::move(resources)));
    }
    return allResources;
}

void UserCommandsHandler::handle(BroadcastCommand *command)
{
    // TODO fragmentation is a big deal, should divide resources into multiple packets to prevent it
    const auto ownedResources = resourceManager.getOwnedResources();
    auto map = convertInfoMapToResourceMap(ownedResources);
    BroadcastMessage message(std::move(map));
    const auto bytes = message.toByteStream();
    socket.writeTo(&bytes[0], bytes.size(), "127.255.255.255", broadcastPort);
    std::stringstream stream;
    log << "--- Sending broadcast." << std::endl;
    stream << "--- Sending broadcast." << std::endl;
    stream << std::to_string(bytes.size()) << "bytes sent." << std::endl;
    commandInterface->sendResponse(stream.str());
}

void UserCommandsHandler::handle(DisplayCommand *command)
{
    log << "Displaying network resources." << std::endl;
    std::stringstream stream;
    stream << "Network resources: " << std::endl;
    const auto resources = resourceManager.getNetworkResources();
    printResourceMap<NetworkResourceInfo>(resources, stream);
    stream << "Owned resources: " << std::endl;
    const auto ownedResources = resourceManager.getOwnedResources();
    printResourceMap<LocalResourceInfo>(ownedResources, stream);
    commandInterface->sendResponse(stream.str());
}

void UserCommandsHandler::handle(AddCommand *command)
{
    std::pair<AuthorKeyType, Resource> resource = resourceFromFile(command->getFileName());
    resourceManager.addOwnedResource(resource.first, resource.second);
    std::stringstream stream;
    stream << "File " << command->getFileName() << " added to local resources.";
    log << stream.str() << std::endl;
    commandInterface->sendResponse(stream.str());
}

void UserCommandsHandler::handle(UnknownCommand *command)
{
    log << "Unknown command." << std::endl;
    commandInterface->sendResponse("Daemon unable to recognize the command");
}

void UserCommandsHandler::handle(DownloadCommand *command)
{
    try
    {
        const auto resource = resourceManager.getResourceById(command->getLocalId());
        log << "Downloading file: " << resource.second.getName() << std::endl;
        commandInterface->sendResponse("Downloading in progress.");
        downloader.downloadResource(std::move(resource));
    }
    catch (const std::runtime_error& e)
    {
        log << e.what() << std::endl;
        commandInterface->sendResponse(e.what());
    }
}



















