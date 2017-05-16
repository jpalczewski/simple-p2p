//
// Created by kamil on 14/05/17.
//

#include <algorithm>
#include "ResourceManager.h"

void ResourceManager::addResource(std::string publicKey, std::unique_ptr<Resource> resource)
{
    std::lock_guard<std::shared_timed_mutex> lock(mutex);
    networkResources[std::move(publicKey)].push_back(std::make_pair(std::move(resource), std::make_unique<NetworkResourceInfo>()));
}

NetworkResourceInfo ResourceManager::getResourceInfo(const std::string &publicKey, const Resource &resource)
{
    std::shared_lock<std::shared_timed_mutex> lock(mutex);
    auto& list = networkResources.find(publicKey)->second;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (*it->first == resource)
            return *it->second;
    }
}



