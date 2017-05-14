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
#include "ResourceInfo.h"
#include <shared_mutex>

class ResourceManager
{
private:
    // TODO use a map instead of a list? but unique_ptr cannot be a map key...
    std::unordered_map<std::string, std::list<std::pair<std::unique_ptr<Resource>, std::unique_ptr<ResourceInfo>>>> resources;
    std::shared_timed_mutex mutex;

public:
    void addResource(std::string publicKey, std::unique_ptr<Resource> resource);
    ResourceInfo& getResourceInfo(const std::string& publicKey, const Resource& resource);
};


#endif //SIMPLE_P2P_RESOURCEMANAGER_H
