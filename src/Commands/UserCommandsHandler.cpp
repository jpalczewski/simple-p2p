//
// Created by kamil on 25/05/17.
//

#include "UserCommandsHandler.h"
#include "../Messages/BroadcastMessage.h"
#include "CommandTypes/AddCommand.h"
#include "CommandTypes/DownloadCommand.h"
#include "CommandTypes/BlockCommand.h"
#include "NetworkCommandInterface.h"

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
                                                     commandInterface(std::make_unique<NetworkCommandInterface>(clientPort))
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

Resource UserCommandsHandler::resourceFromFile(std::string fileName)
{
    int size = 1111;
    const char *testHash = "0123456789012345";
    unsigned char *hashBytes = (unsigned char *) testHash;
    std::vector<unsigned char> hash(testHash, testHash + 16);
    std::vector<unsigned char> sign(128, 0x55);
    return Resource(fileName, size, hash, sign);
}

std::string UserCommandsHandler::readPublicKey()
{
    return "-----BEGIN RSA PUBLIC KEY-----\n"
            "MIGJAoGBANLVDLEKgzjGBqiNPTqRqPQynEbbNZbY2T2K+Fn3YMX8hbZ8QLn48CZI\n"
            "n4mKDQSVND1qxBCeRBxYeaavwyZ8LCqVYPCXZwkRH0QlyReQOCxyw7duEIl/F7tc\n"
            "N3/34qht2DK8lOp9IQS7sY9L5Oy73JW4jQUMcPUUg52bq7pzccWFAgMBAAE=\n"
            "-----END RSA PUBLIC KEY-----\n";
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
    socket.writeTo(&bytes[0], bytes.size(), "127.255.255.255", broadcastPort);
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
    Resource resource = resourceFromFile(command->getFileName());
    std::string publicKey = readPublicKey();
    resourceManager.addOwnedResource(publicKey, resource);
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
        commandInterface->sendResponse("Not yet implemented. Stay tuned.");
    }
    catch (const std::runtime_error& e)
    {
        log << e.what() << std::endl;
        commandInterface->sendResponse(e.what());
    }
}

void UserCommandsHandler::handle(OneIntegerParamCommand *command) {
    if(command->getType()==Command::Type::Block)
        handle(static_cast<BlockCommand*>(command));
    else
        throw std::runtime_error("It shouldn't come here");
}

void UserCommandsHandler::handle(BlockCommand *command) {
    auto result = resourceManager.getResourceById(command->getLocalId());
    resourceManager.setOwnedResourceInfoState(result.first, result.second, Resource::State::Blocked);
    std::stringstream stream;
    stream << "File " << result.first << " is blocked.";
    broadcastOnDemand();
    log << stream.str() << std::endl;
    commandInterface->sendResponse(stream.str());
}



















