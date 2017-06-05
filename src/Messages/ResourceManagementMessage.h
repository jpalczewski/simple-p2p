//
// Created by kamil on 04/06/17.
//

#ifndef SIMPLE_P2P_RESOURCEMANAGEMENTMESSAGE_H
#define SIMPLE_P2P_RESOURCEMANAGEMENTMESSAGE_H

#include "../Resources/Resource.h"
#include "../Files/FileManagerTypes.h"

class ResourceManagementMessage
{
public:
    ResourceManagementMessage(const AuthorKeyType &publicKey, const Resource &resource,
                              const std::vector<unsigned char> &sign) : publicKey(publicKey), resource(resource),
                                                                        sign(sign)
    { }

    static ResourceManagementMessage fromByteStream(std::vector<unsigned char> byteArray, int currentIndex);
    std::vector<unsigned char> toByteStream(bool withSignature = true) const;


    const AuthorKeyType &getPublicKey() const
    {
        return publicKey;
    }

    const Resource &getResource() const
    {
        return resource;
    }

    const std::vector<unsigned char> &getSign() const
    {
        return sign;
    }

    void setSign(const std::vector<unsigned char> &sign);

private:
    AuthorKeyType publicKey;
    Resource resource;
    std::vector<unsigned char> sign;
};


#endif //SIMPLE_P2P_RESOURCEMANAGEMENTMESSAGE_H
