//
// Created by kamil on 16/05/17.
//

#ifndef SIMPLE_P2P_LOCALRESOURCEINFO_H
#define SIMPLE_P2P_LOCALRESOURCEINFO_H


class LocalResourceInfo
{
    enum class State {Active, Blocked, Invalid};

    State state;

    // TODO: more resource properties
};


#endif //SIMPLE_P2P_LOCALRESOURCEINFO_H
