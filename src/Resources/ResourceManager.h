//
// Created by kamil on 14/05/17.
//

#ifndef SIMPLE_P2P_RESOURCEMANAGER_H
#define SIMPLE_P2P_RESOURCEMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include "Resource.h"
#include "NetworkResourceInfo.h"
#include "LocalResourceInfo.h"
#include <shared_mutex>

class ResourceManager
{
private:
    // TODO use a map instead of a list? but unique_ptr cannot be a map key...

    // network resources - available for download
    std::unordered_map<std::string, std::list<std::pair<std::unique_ptr<Resource>, std::unique_ptr<NetworkResourceInfo>>>> networkResources;

    // resources downloaded from other nodes - can be shared, but can't be blocked/invalidated/deleted
    std::unordered_map<std::string, std::list<std::pair<std::unique_ptr<Resource>, std::unique_ptr<LocalResourceInfo>>>> localResources;

    // owned resources - can be shared and blocked/invalidated/deleted
    std::unordered_map<std::string, std::list<std::pair<std::unique_ptr<Resource>, std::unique_ptr<LocalResourceInfo>>>> ownedResources;

    // will probably need 3 different mutexes for 3 maps
    std::shared_timed_mutex mutex;

public:
    void addResource(std::string publicKey, std::unique_ptr<Resource> resource);
    NetworkResourceInfo getResourceInfo(const std::string& publicKey, const Resource& resource);
};


#endif //SIMPLE_P2P_RESOURCEMANAGER_H
