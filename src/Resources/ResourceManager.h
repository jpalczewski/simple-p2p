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
public:
    void addNetworkResource(const std::string &publicKey, const Resource &resource, const NetworkResourceInfo& info = NetworkResourceInfo());
    NetworkResourceInfo getNetworkResourceInfo(const std::string &publicKey, const Resource &resource);

    void addLocalResource(const std::string &publicKey, const Resource &resource);
    LocalResourceInfo getLocalResourceInfo(const std::string &publicKey, const Resource &resource);

    void addOwnedResource(const std::string &publicKey, const Resource &resource);
    LocalResourceInfo getOwnedResourceInfo(const std::string &publicKey, const Resource &resource);

    template <typename Type>
    using ResourceMap = std::unordered_map<std::string, std::unordered_map<Resource, Type, ResourceHash>>;

    ResourceManager::ResourceMap<NetworkResourceInfo> getNetworkResources();
    ResourceManager::ResourceMap<LocalResourceInfo> getOwnedResources();
private:
    // network resources - available for download
    ResourceMap<NetworkResourceInfo> networkResources;

    // resources downloaded from other nodes - can be shared, but can't be blocked/invalidated/deleted
    ResourceMap<LocalResourceInfo> localResources;

    // owned resources - can be shared and blocked/invalidated/deleted
    ResourceMap<LocalResourceInfo> ownedResources;

    std::shared_timed_mutex localMutex;
    std::shared_timed_mutex ownedMutex;
    std::shared_timed_mutex networkMutex;
};

extern ResourceManager resourceManager;

#endif //SIMPLE_P2P_RESOURCEMANAGER_H
