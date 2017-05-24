//
// Created by kamil on 14/05/17.
//

#include <algorithm>
#include "ResourceManager.h"

namespace
{
     //TODO check whether resource exists in this map or not
    template <typename T>
    void addResource(const std::string &publicKey, const Resource &resource, ResourceManager::ResourceMap<T>& map,
                     const T& info = T{})
    {
        map[publicKey].insert(std::make_pair(resource, T{}));
    }

    // TODO check if first (publicKey as key) map is found
    template <typename T>
    T getResourceInfo(const std::string &publicKey, const Resource &resource, ResourceManager::ResourceMap<T>& map)
    {
        const auto& publicKeyMap = map.find(publicKey)->second;
        return publicKeyMap.find(resource)->second;
    }

    void addNetworkResource(const std::string &publicKey, const Resource &resource,
                     ResourceManager::ResourceMap<NetworkResourceInfo>& map,
                     const NetworkResourceInfo& info)
    {
        auto &keyMap = map[publicKey];
        const auto res = keyMap.find(resource);
        if (res == keyMap.end())
            keyMap[resource] = info;
        else
            res->second.addSeeders(info.getSeeders());
    }
}

void ResourceManager::addNetworkResource(const std::string &publicKey, const Resource &resource,
                                         const NetworkResourceInfo& info)
{
    std::lock_guard<std::shared_timed_mutex> lock(networkMutex);
    ::addNetworkResource(publicKey, resource, networkResources, info);
}

NetworkResourceInfo ResourceManager::getNetworkResourceInfo(const std::string &publicKey, const Resource &resource)
{
    std::shared_lock<std::shared_timed_mutex> lock(networkMutex);
    return ::getResourceInfo<NetworkResourceInfo>(publicKey, resource, networkResources);
}

void ResourceManager::addLocalResource(const std::string &publicKey, const Resource &resource)
{
    std::lock_guard<std::shared_timed_mutex> lock(localMutex);
    ::addResource<LocalResourceInfo>(publicKey, resource, localResources);
}

LocalResourceInfo ResourceManager::getLocalResourceInfo(const std::string &publicKey, const Resource &resource)
{
    std::shared_lock<std::shared_timed_mutex> lock(localMutex);
    return ::getResourceInfo<LocalResourceInfo>(publicKey, resource, localResources);
}

void ResourceManager::addOwnedResource(const std::string &publicKey, const Resource &resource)
{
    std::lock_guard<std::shared_timed_mutex> lock(ownedMutex);
    ::addResource<LocalResourceInfo>(publicKey, resource, ownedResources);
}

LocalResourceInfo ResourceManager::getOwnedResourceInfo(const std::string &publicKey, const Resource &resource)
{
    std::shared_lock<std::shared_timed_mutex> lock(ownedMutex);
    return ::getResourceInfo<LocalResourceInfo>(publicKey, resource, ownedResources);
}

ResourceManager::ResourceMap<NetworkResourceInfo> ResourceManager::getNetworkResources()
{
    std::shared_lock<std::shared_timed_mutex> lock(networkMutex);
    return networkResources;
}

ResourceManager resourceManager;







