//
// Created by kamil on 14/05/17.
//

#include <algorithm>
#include "ResourceManager.h"

uint64_t ResourceManager::lastLocalId = 0;

namespace
{
    template <typename T>
    bool addResource(const std::string &publicKey,
                     const Resource &resource,
                    // uint64_t id,
                     ResourceManager::ResourceMap<T>& map,
                     const T& info = T())
    {
        //info.setLocalId(id);
        auto insertResult = map[publicKey].insert(std::make_pair(resource, info));
        return insertResult.second;
    }
    
    template <typename T>
    T getResourceInfo(const std::string &publicKey, const Resource &resource, ResourceManager::ResourceMap<T>& map)
    {
        auto keyIterator = map.find(publicKey);
        if (keyIterator == map.end())
            throw std::runtime_error("Cannot get resource info for " + resource.getName() + " - key not found");
         const auto& publicKeyMap = keyIterator->second;
        auto resourceFound = publicKeyMap.find(resource);
        if (resourceFound == publicKeyMap.end())
            throw std::runtime_error("Cannot get resource info for " + resource.getName() + " - resource not found");
        return resourceFound->second;
    }

    template <typename T>
    bool setResourceInfoState(const std::string &publicKey, const Resource &resource,
                              ResourceManager::ResourceMap<T> &map, Resource::State newResourceState)
    {
        auto keyFound = map.find(publicKey);
        if (keyFound == map.end())
            throw std::runtime_error("Cannot set resource info for " + resource.getName() + " - key not found");
        auto& publicKeyMap = keyFound->second;
        auto findResult = publicKeyMap.find(resource);
        if(findResult == publicKeyMap.end())
            throw std::runtime_error("Resource not found in setResourceInfoState");
        auto oldState = findResult->second.getResourceState();
        if(oldState==Resource::State::Invalid)
            throw std::runtime_error("You shouldn't mess with invalid resource!");


        findResult->second.setResourceState(newResourceState);
    }
}

void ResourceManager::addNetworkResource(const std::string &publicKey, const Resource &resource,
                                         const std::vector<IpAddress>& seeders)
{
    std::lock_guard<std::shared_timed_mutex> lock2(localIdMutex);
    std::lock_guard<std::shared_timed_mutex> lock(networkMutex);

    auto &keyMap = networkResources[publicKey];
    auto keyMapIterator = networkResources.find(publicKey);
    auto localId = lastLocalId++;
    NetworkResourceInfo info{localId};
    auto inserted = keyMap.insert(std::make_pair(resource, info));
    if (inserted.second)
        localIdsMap[localId] = std::make_pair(&keyMapIterator->first, &inserted.first->first);
    else
        --lastLocalId; // no new resource, and we incremented last id earlier so now need to decrement it
    inserted.first->second.addSeeders(seeders);
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
    std::lock_guard<std::shared_timed_mutex> lock(localIdMutex);
    std::lock_guard<std::shared_timed_mutex> lock2(ownedMutex);
    auto result = ::addResource<LocalResourceInfo>(publicKey, resource, ownedResources);
    if(result)
    {
        auto & keyMap = ownedResources[publicKey];
        auto & info = keyMap[resource];
        info.setLocalId(lastLocalId);
//      keyMap[resource] = LocalResourceInfo(lastLocalId);

        auto keyMapIterator = ownedResources.find(publicKey);
//       NetworkResourceInfo info{localId};
        auto inserted = keyMap.find(resource);
        localIdsMap[lastLocalId] = std::make_pair(&keyMapIterator->first, &inserted->first);
        lastLocalId++;
    }
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

ResourceManager::ResourceMap<LocalResourceInfo> ResourceManager::getOwnedResources()
{
    std::shared_lock<std::shared_timed_mutex> lock(ownedMutex);
    return ownedResources;
}

std::pair<std::string, Resource> ResourceManager::getResourceById(uint64_t id)
{
    std::shared_lock<std::shared_timed_mutex> lock(networkMutex);
    const auto& found = localIdsMap.find(id);
    if (found == localIdsMap.end()) {
        std::stringstream ss;
        ss << "Requested resource(" << id << ") not found in getResourceById";
        throw std::runtime_error(ss.str());
    }
    const auto& entry = found->second;
    return std::make_pair(*entry.first, *entry.second);
}

void ResourceManager::setOwnedResourceInfoState(const std::string &publicKey, const Resource &resource, const Resource::State &newState) {
    std::lock_guard<std::shared_timed_mutex> lock(ownedMutex);
    ::setResourceInfoState(publicKey, resource, ownedResources, newState);
}

void ResourceManager::setNetworkResourceInfoState(const std::string &publicKey, const Resource &resource,
                                                  const Resource::State &newState)
{
    std::lock_guard<std::shared_timed_mutex> lock(networkMutex);
    ::setResourceInfoState(publicKey, resource, networkResources, newState);
}

void ResourceManager::trySetLocalResourceInfoState(const std::string &publicKey, const Resource &resource,
                                                   Resource::State state)
{
    std::lock_guard<std::shared_timed_mutex> lock(localMutex);
    try
    {
        ::setResourceInfoState(publicKey, resource, networkResources, state);
    }
    catch (const std::runtime_error& e)
    {
        // we don't have this resource locally so we don't have to do anything
        // not the prettiest solution...
    }
}

LocalResourceInfo ResourceManager::getLocalOrSharedResourceInfo(const std::string &publicKey, const Resource &resource)
{
    std::shared_lock<std::shared_timed_mutex> lock(localMutex);
    std::shared_lock<std::shared_timed_mutex> lock2(ownedMutex);
    try
    {
        return ::getResourceInfo(publicKey, resource, ownedResources);
    }
        // quite ugly but...
    catch (const std::runtime_error& e)
    {
        return ::getResourceInfo(publicKey, resource, localResources);
    }
}

ResourceManager::ResourceMap<LocalResourceInfo> ResourceManager::getLocalResources()
{
    std::shared_lock<std::shared_timed_mutex> lock(localMutex);
    return localResources;
}

void ResourceManager::deleteOwnedResource(const std::string &publicKey, const Resource &resource)
{
    std::lock_guard<std::shared_timed_mutex> lock(ownedMutex);
    auto keyFound = ownedResources.find(publicKey);
    if (keyFound == ownedResources.end())
        throw std::runtime_error("Cannot delete owned resource - key not found");
    keyFound->second.erase(resource);
    if (keyFound->second.empty())
        ownedResources.erase(keyFound);
}

void ResourceManager::deleteSharedAndNetworkResource(const std::string &publicKey, const Resource &resource)
{
    std::lock_guard<std::shared_timed_mutex> lock(networkMutex);
    std::lock_guard<std::shared_timed_mutex> lock2(localMutex);
    auto keyFound = networkResources.find(publicKey);
    if (keyFound != networkResources.end())
    {
        keyFound->second.erase(resource);
        if (keyFound->second.empty())
            networkResources.erase(keyFound);
    }

    auto keyFound2 = localResources.find(publicKey);
    if (keyFound2 != localResources.end())
    {
        keyFound2->second.erase(resource);
        if (keyFound2->second.empty())
            localResources.erase(keyFound2);
    }
}


ResourceManager resourceManager;







