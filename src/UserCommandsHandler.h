//
// Created by kamil on 25/05/17.
//

#ifndef SIMPLE_P2P_USERCOMMANDSHANDLER_H
#define SIMPLE_P2P_USERCOMMANDSHANDLER_H


#include <unordered_map>
#include "Resources/Resource.h"
#include "Network/Socket.h"
#include "Resources/LocalResourceInfo.h"
#include "Resources/ResourceManager.h"

class UserCommandsHandler
{
public:
    UserCommandsHandler(int port);
    virtual ~UserCommandsHandler();

    void handleUserInput();

private:
    int port;
    Socket socket;

    void handleAdd();

    void handleDisplay();

    void handleBroadcast();

    Resource resourceFromFile(std::string basic_string);

    std::string readPublicKey();

    std::unordered_map<std::string, std::vector<Resource>> convertInfoMapToResourceMap(ResourceManager::ResourceMap<LocalResourceInfo>);
};

#endif //SIMPLE_P2P_USERCOMMANDSHANDLER_H
