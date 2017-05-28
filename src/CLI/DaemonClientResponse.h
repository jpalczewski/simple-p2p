//
// Created by kamil on 28/05/17.
//

#ifndef SIMPLE_P2P_DAEMONCLIENTRESPONSE_H
#define SIMPLE_P2P_DAEMONCLIENTRESPONSE_H

#include <string>

class DaemonClientResponse
{
public:
    DaemonClientResponse(const std::string &content) : content(content)
    { }

    const std::string &getContent() const
    {
        return content;
    }

private:
    const std::string content;
};

#endif //SIMPLE_P2P_DAEMONCLIENTRESPONSE_H
