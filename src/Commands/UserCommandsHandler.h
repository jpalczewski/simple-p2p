//
// Created by kamil on 25/05/17.
//

#ifndef SIMPLE_P2P_USERCOMMANDSHANDLER_H
#define SIMPLE_P2P_USERCOMMANDSHANDLER_H


#include <unordered_map>
#include "../Resources/Resource.h"
#include "../Network/Socket.h"
#include "../Resources/LocalResourceInfo.h"
#include "../Resources/ResourceManager.h"
#include "Visitor.h"
#include "CommandInterface.h"

class UserCommandsHandler : public Visitor
{
public:
    UserCommandsHandler(int port);
    virtual ~UserCommandsHandler();

    void handleUserInput();

    void handle(BroadcastCommand* command);
    void handle(DisplayCommand* command);
    void handle(AddCommand* command);
    void handle(UnknownCommand* command);

private:
    int port;
    Socket socket;
    std::unique_ptr<CommandInterface> commandInterface;

    Resource resourceFromFile(std::string basic_string);
    std::string readPublicKey();
    std::unordered_map<std::string, std::vector<Resource>> convertInfoMapToResourceMap(ResourceManager::ResourceMap<LocalResourceInfo>);
};

#endif //SIMPLE_P2P_USERCOMMANDSHANDLER_H