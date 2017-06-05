//
// Created by kamil on 25/05/17.
//

#ifndef SIMPLE_P2P_USERCOMMANDSHANDLER_H
#define SIMPLE_P2P_USERCOMMANDSHANDLER_H


#include <unordered_map>
#include <iostream>
#include "../Resources/Resource.h"
#include "../Network/Socket.h"
#include "../Resources/LocalResourceInfo.h"
#include "../Resources/ResourceManager.h"
#include "Visitor.h"
#include "CommandInterface.h"
#include "../Files/FileManagerTypes.h"
#include "ResourceDownloadHandler.h"
#include "../Messages/MessageType.h"

class UserCommandsHandler : public Visitor
{
public:
    UserCommandsHandler(int broadCastPort, int clientPort);
    virtual ~UserCommandsHandler();

    void handleUserInput();

    void handle(BroadcastCommand* command);
    void handle(DisplayCommand* command);
    void handle(AddCommand* command);
    void handle(UnknownCommand* command);
    void handle(DownloadCommand* command);
    void handle(BlockCommand* command);
    void handle(UnblockCommand *command);
    void handle(DeleteCommand *command);
    void handle(InvalidateCommand *command);
    void handle(CancelCommand *command);

private:
    int broadcastPort;
    Socket socket;
    std::unique_ptr<CommandInterface> commandInterface;
    ResourceDownloadHandler downloader;
    std::ostream& log = std::cout;

    std::pair<AuthorKeyType, Resource> resourceFromFile(std::string filePath);
    std::unordered_map<std::string, std::vector<Resource>> convertInfoMapToResourceMap(ResourceManager::ResourceMap<LocalResourceInfo>);

    std::stringstream broadcastOnDemand();

    void sendBroadcastMessage(const std::pair<std::string, Resource> &result, const MessageType &messageType);

    void sendBroadcastMessage(uint64_t id, const MessageType &messageType, const Resource::State &state);

    void broadcastResourceMap(std::unordered_map<std::string, std::vector<Resource>> &map);
};

#endif //SIMPLE_P2P_USERCOMMANDSHANDLER_H
