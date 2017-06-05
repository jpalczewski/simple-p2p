//
// Created by kamil on 25/05/17.
//

#include "UserCommandsHandler.h"
#include "../Messages/BroadcastMessage.h"
#include "CommandTypes/AddCommand.h"
#include "CommandTypes/DownloadCommand.h"
#include "CommandTypes/BlockCommand.h"
#include "NetworkCommandInterface.h"
#include "ResourceDownloadHandler.h"
#include "../Files/FileManager.h"
#include "../Messages/ResourceManagementMessage.h"
#include "CommandTypes/UnblockCommand.h"
#include "CommandTypes/InvalidateCommand.h"
#include "CommandTypes/DeleteCommand.h"

#include "../ConfigHandler.h"

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

std::pair<AuthorKeyType, Resource> UserCommandsHandler::resourceFromFile(std::string filePath)
{
    const std::string publicKeyFileName = ConfigHandler::getInstance()->get("keys.dir")+"rsa_public.pem";
    const std::string privateKeyFileName = ConfigHandler::getInstance()->get("keys.dir")+"rsa_private.pem";
    AuthorKey key(publicKeyFileName, privateKeyFileName);
    AddFileRequest request(key.getPublicKey(), key.getPrivateKey(), filePath);
    auto hashAndSign = fileManagerInstance.addFile(request);
    size_t size = file_size(filePath);
    std::string fileName = path(filePath).filename().string();
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
    std::stringstream stream = broadcastOnDemand();
    commandInterface->sendResponse(stream.str());
}

std::stringstream UserCommandsHandler::broadcastOnDemand() {
    // TODO fragmentation is a big deal, should divide resources into multiple packets to prevent it
    const auto ownedResources = resourceManager.getOwnedResources();
    auto map = convertInfoMapToResourceMap(ownedResources);
    BroadcastMessage message(move(map));
    const auto bytes = message.toByteStream();
    socket.writeTo(&bytes[0], bytes.size(), ConfigHandler::getInstance()->get("network.broadcast_ip"), broadcastPort);
    std::stringstream stream;
    log << "--- Sending broadcast." << std::endl;
    stream << "--- Sending broadcast." << std::endl;
    stream << std::__cxx11::to_string(bytes.size()) << "bytes sent." << std::endl;
    return stream;
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


void UserCommandsHandler::handle(BlockCommand *command) {
    auto result = resourceManager.getResourceById(command->getLocalId());
    resourceManager.setOwnedResourceInfoState(result.first, result.second, Resource::State::Blocked);
    std::stringstream stream;
    stream << "File " << result.first << " is blocked.";
    std::vector<unsigned char> sign(128, 0x44);
    ResourceManagementMessage message(result.first, result.second, sign);
    std::vector<unsigned char> binaryMessage {static_cast<unsigned char>(MessageType::BlockResource)};
    auto messageData = message.toByteStream();
    binaryMessage.insert(binaryMessage.end(), messageData.begin(), messageData.end());
    socket.writeTo(&binaryMessage[0], binaryMessage.size(), ConfigHandler::getInstance()->get("network.broadcast_ip"), broadcastPort);
    log << stream.str() << std::endl;
    commandInterface->sendResponse(stream.str());
}

void UserCommandsHandler::handle(UnblockCommand *command) {
    std::stringstream stream;
    stream << "Unblock" << std::endl;
    log << stream.str() << std::endl;
    commandInterface->sendResponse(stream.str());
}

void UserCommandsHandler::handle(DeleteCommand *command) {
    std::stringstream stream;
    stream << "Delete" << std::endl;
    log << stream.str() << std::endl;
    commandInterface->sendResponse(stream.str());
}

void UserCommandsHandler::handle(InvalidateCommand *command) {
    std::stringstream stream;
    stream << "Invalidate" << std::endl;
    log << stream.str() << std::endl;
    commandInterface->sendResponse(stream.str());
}

void UserCommandsHandler::handle(CancelCommand *command) {
    std::stringstream stream;
    stream << "Cancel" << std::endl;
    log << stream.str() << std::endl;
    commandInterface->sendResponse(stream.str());
}



















