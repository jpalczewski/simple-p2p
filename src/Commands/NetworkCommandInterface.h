//
// Created by kamil on 28/05/17.
//

#ifndef SIMPLE_P2P_NETWORKCOMMANDINTERFACE_H
#define SIMPLE_P2P_NETWORKCOMMANDINTERFACE_H


#include <boost/archive/binary_iarchive.hpp>
#include "CommandInterface.h"
#include "../Network/Socket.h"
#include "CommandTypes/Command.h"

class NetworkCommandInterface : public CommandInterface
{
public:
    NetworkCommandInterface(int listenPort);

    std::unique_ptr<Command> getNextCommand();
    void sendResponse(std::string response);

    template<typename T>
    std::unique_ptr<Command> unzipCommand(std::stringstream & stream)
    {
        static_assert(std::is_base_of<Command, T>::value, "Wrong template class");
        boost::archive::binary_iarchive archive(stream);
        T command;
        archive >> command;
        return std::make_unique<T>(command);
    }

private:
    Socket socket;
    const int listenPort;
    IpAddress lastSender;

    Command::Type safeCast(unsigned char input);
};

#endif //SIMPLE_P2P_NETWORKCOMMANDINTERFACE_H
