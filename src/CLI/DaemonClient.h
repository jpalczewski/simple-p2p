//
// Created by kamil on 28/05/17.
//

#ifndef SIMPLE_P2P_DAEMONCLIENT_H
#define SIMPLE_P2P_DAEMONCLIENT_H

#include <string>
#include <boost/archive/binary_oarchive.hpp>
#include "DaemonClientResponse.h"
#include "../Network/Socket.h"
#include "../Commands/Command.h"
#include "../Commands/OneIntegerParamCommand.h"
#include "../Commands/BlockCommand.h"
#include "../Commands/UnblockCommand.h"
class DaemonClient
{
public:
    DaemonClient(std::string daemonAddress, int daemonPort);

    DaemonClientResponse sendAdd(std::string filePath);
    DaemonClientResponse sendBroadcast();
    DaemonClientResponse sendDisplay();
    DaemonClientResponse sendDownload(uint64_t localId);

    template <class O>
    DaemonClientResponse sendOneParam(uint64_t localId) {
        O command(localId);
        std::stringstream stream;
        boost::archive::binary_oarchive archive(stream);

        archive << command;
        std::string message(1, static_cast<char>(command.getType()));
        message += stream.str();
        return sendMessage(std::move(message));
    }

private:
    Socket socket;

    std::string daemonAddress;
    int daemonPort;

    DaemonClientResponse sendMessage(std::string message);
};

#endif //SIMPLE_P2P_DAEMONCLIENT_H
