//
// Created by kamil on 28/05/17.
//

#include <vector>
#include <sstream>
#include <iterator>
#include "NetworkCommandInterface.h"
#include "AddCommand.h"
#include "DisplayCommand.h"
#include "BroadcastCommand.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

NetworkCommandInterface::NetworkCommandInterface() : socket(Socket::Domain::Ip4, Socket::Type::Udp)
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
    throw std::runtime_error("Received unknown command");
}

void NetworkCommandInterface::sendResponse(std::string response)
{
    socket.writeTo(reinterpret_cast<const unsigned char*>(response.c_str()), response.length(), lastSender.getAddress(), lastSender.getPort());
}





