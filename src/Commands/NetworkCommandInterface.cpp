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
#include "CommandTypes/StatusCommand.h"
#include "CommandTypes/UnblockCommand.h"
#include "CommandTypes/InvalidateCommand.h"
#include "CommandTypes/CancelCommand.h"
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
    switch(safeCast(buffer[0]))
    {
        case Command::Type::Display:
            return std::make_unique<DisplayCommand>();
        case Command::Type::Broadcast:
            return std::make_unique<BroadcastCommand>();
        case Command::Type::Status:
            return std::make_unique<StatusCommand>();
        case Command::Type::Unknown:
            throw std::runtime_error("Unknown error received");

        //more demanding cases
        case Command::Type::Add:
            return unzipCommand<AddCommand>(stream);
        case Command::Type::Download:
            return unzipCommand<DownloadCommand>(stream);
        case Command::Type::Block:
            return unzipCommand<BlockCommand>(stream);
        case Command::Type::Unblock:
            return unzipCommand<UnblockCommand>(stream);
        case Command::Type::Cancel:
            return unzipCommand<CancelCommand>(stream);
        case Command::Type::Invalidate:
            return unzipCommand<InvalidateCommand>(stream);
        case Command::Type::Delete:
            return unzipCommand<DownloadCommand>(stream);

    }
}

void NetworkCommandInterface::sendResponse(std::string response)
{
    socket.writeTo(reinterpret_cast<const unsigned char*>(response.c_str()), response.length(), lastSender.getAddress(), lastSender.getPort());
}

Command::Type NetworkCommandInterface::safeCast(unsigned char input) {
    Command::Type type = static_cast<Command::Type>(input);
    switch(type)
    {
        case Command::Type::Broadcast:break;
        case Command::Type::Display:break;
        case Command::Type::Add:break;
        case Command::Type::Download:break;
        case Command::Type::Block:break;
        case Command::Type::Unblock:break;
        case Command::Type::Cancel:break;
        case Command::Type::Invalidate:break;
        case Command::Type::Delete:break;
        case Command::Type::Status:break; //I just love clion
        default:
            return Command::Type::Unknown;
    }
    return type;
}





