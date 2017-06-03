//
// Created by kamil on 28/05/17.
//

#include <vector>
#include <sstream>
#include <iterator>
#include "NetworkCommandInterface.h"
#include "CommandTypes/AddCommand.h"
#include "CommandTypes/DisplayCommand.h"
#include "CommandTypes/BroadcastCommand.h"
#include "CommandTypes/DownloadCommand.h"
#include "CommandTypes/BlockCommand.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

NetworkCommandInterface::NetworkCommandInterface(int listenPort)
        : socket(Socket::Domain::Ip4, Socket::Type::Udp), listenPort(listenPort)
{
    socket.bind(listenPort);
}

std::unique_ptr<Command> NetworkCommandInterface::getNextCommand()
{
    // TODO receive messages bigger than 1024 bytes
    std::vector<unsigned char> buffer(1024);
    socket.readFrom(&buffer[0], 1024, lastSender);
    std::stringstream stream;
    std::copy(buffer.begin() + 1, buffer.end(), std::ostream_iterator<unsigned char>(stream));
    switch(static_cast<Command::Type>(buffer[0]))
    {
        case Command::Type::Display:
            return std::make_unique<DisplayCommand>();

    }
    if (buffer[0] == static_cast<unsigned char>(Command::Type::Display))
        return std::make_unique<DisplayCommand>();
    if (buffer[0] == static_cast<unsigned char>(Command::Type::Broadcast))
        return std::make_unique<BroadcastCommand>();

    if (buffer[0] == static_cast<unsigned char>(Command::Type::Add))
    {
        boost::archive::binary_iarchive archive(stream);
        AddCommand command;
        archive >> command;
        return std::make_unique<AddCommand>(command);
    }

    if (buffer[0] == static_cast<unsigned char>(Command::Type::Download))
    {
        boost::archive::binary_iarchive archive(stream);
        DownloadCommand command;
        archive >> command;
        return std::make_unique<DownloadCommand>(command);
    }
    if (buffer[0] == static_cast<unsigned char>(Command::Type::Block))
    {
        boost::archive::binary_iarchive archive(stream);
        BlockCommand command;
        archive >> command;
        return std::make_unique<BlockCommand>(command);
    }

    throw std::runtime_error("Received unknown command");
}

void NetworkCommandInterface::sendResponse(std::string response)
{
    socket.writeTo(reinterpret_cast<const unsigned char*>(response.c_str()), response.length(), lastSender.getAddress(), lastSender.getPort());
}





