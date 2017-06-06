//
// Created by kamil on 28/05/17.
//

#include <sstream>
#include "DaemonClient.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

DaemonClient::DaemonClient(std::string daemonAddress, int daemonPort) : socket(Socket::Domain::Ip4, Socket::Type::Udp),
                                                                        daemonAddress(daemonAddress), daemonPort(daemonPort)
{ 
    socket.bind(0);
    socket.setTimeout(5);
}

DaemonClientResponse DaemonClient::sendAdd(std::string filePath)
{
    AddCommand command(std::move(filePath));
    std::stringstream stream;
    boost::archive::binary_oarchive archive(stream);
    archive << command;
    std::string message(1, static_cast<char>(Command::Type::Add));
    message += stream.str();
    return sendMessage(std::move(message));
}


DaemonClientResponse DaemonClient::sendMessage(std::string message)
{
    socket.writeTo(reinterpret_cast<const unsigned char*>(message.data()), message.size(), daemonAddress, daemonPort);
    std::vector<unsigned char> response(1024);
    if (socket.readFrom(&response[0], 1024, daemonAddress) < 0)
        throw std::runtime_error("Couldn't get any response from daemon");
    return DaemonClientResponse(std::string(reinterpret_cast<char*>(response.data())));
}













