//
// Created by kamil on 18/05/17.
//

#ifndef SIMPLE_P2P_MESSAGETYPE_H
#define SIMPLE_P2P_MESSAGETYPE_H

enum class MessageType : unsigned char
{
    BroadcastResource = 0x00,
    ResourceRequest = 0x01,
    DeleteResource = 0x02,
    InvalidateResource = 0x03,
    BlockResource = 0x04,
    UnblockResource = 0x05,
    Error = 0x10,
    SendResource = 0x12
};

struct EnumHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

#endif //SIMPLE_P2P_MESSAGETYPE_H
