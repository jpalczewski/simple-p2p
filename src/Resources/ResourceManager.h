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
    NetworkResourceInfo getNetworkResourceInfo(const std::string &publicKey, const Resource &resource);

    void addLocalResource(const std::string &publicKey, const Resource &resource);
    LocalResourceInfo getLocalResourceInfo(const std::string &publicKey, const Resource &resource);


    void addOwnedResource(const std::string &publicKey, const Resource &resource);
    LocalResourceInfo getOwnedResourceInfo(const std::string &publicKey, const Resource &resource);
    void
    setOwnedResourceInfoState(const std::string &publicKey, const Resource &resource, const Resource::State &newState);

    std::pair<std::string, Resource> getResourceById(uint64_t id);
    void addNetworkResource(const std::string &publicKey, const Resource &resource, const std::vector<IpAddress> &seeders);

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

    // we need a convenient way for user to identify resources - full identifier with
    // key, hash, name and so on is too long so we create a local id which is valid only in this node
    std::unordered_map<uint64_t, std::pair<const std::string*, const Resource*>> localIdsMap;

    static uint64_t lastLocalId;

    std::shared_timed_mutex localMutex;
    std::shared_timed_mutex ownedMutex;
    std::shared_timed_mutex networkMutex;
    std::shared_timed_mutex localIdMutex;
};

extern ResourceManager resourceManager;

#endif //SIMPLE_P2P_RESOURCEMANAGER_H
