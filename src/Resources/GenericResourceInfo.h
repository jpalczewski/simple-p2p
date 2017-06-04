//
// Created by erxyi on 03.06.17.
//

#ifndef SIMPLE_P2P_GENERICRESOURCEINFO_H
#define SIMPLE_P2P_GENERICRESOURCEINFO_H
#include "Resource.h"

class GenericResourceInfo {
public:
    GenericResourceInfo(Resource::State resourceState) : resourceState(resourceState) {}
    GenericResourceInfo() :resourceState(Resource::State::Active) {}
    GenericResourceInfo(uint64_t localId, Resource::State resourceState) : localId(localId),
                                                                            resourceState(resourceState) {}
    GenericResourceInfo(uint64_t localId) : localId(localId) {}

    Resource::State getResourceState() const {
        return resourceState;
    }

    void setResourceState(Resource::State resourceState) {
        GenericResourceInfo::resourceState = resourceState;
    }

    uint64_t getLocalId() const {
        return localId;
    }

    void setLocalId(uint64_t localId) {
        GenericResourceInfo::localId = localId;
    }

private:
    uint64_t localId;
    Resource::State resourceState;
};


#endif //SIMPLE_P2P_GENERICRESOURCEINFO_H
