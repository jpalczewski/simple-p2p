//
// Created by kamil on 28/05/17.
//

#ifndef SIMPLE_P2P_DAEMONCLIENT_H
#define SIMPLE_P2P_DAEMONCLIENT_H

#include <string>
#include <boost/archive/binary_oarchive.hpp>
#include "DaemonClientResponse.h"
#include "../Network/Socket.h"
#include "../Commands/CommandTypes/Command.h"
#include "../Commands/CommandTypes/OneIntegerParamCommand.h"
#include "../Commands/CommandTypes/BlockCommand.h"
#include "../Commands/CommandTypes/UnblockCommand.h"
#include "../Commands/CommandTypes/BroadcastCommand.h"
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
        static_assert(std::is_base_of<OneIntegerParamCommand, O>::value, "Wrong class");

        O command(localId);
        std::stringstream stream;
        boost::archive::binary_oarchive archive(stream);
        archive << command;
        std::string message(1, static_cast<char>(command.getType()));
        message += stream.str();
        return sendMessage(std::move(message));
    }

    template <class T>
    DaemonClientResponse sendNoParam() {
        T command;
        std::string message(1, static_cast<char>(command.getType()));
        return sendMessage(std::move(message));
    }


private:
    Socket socket;

    std::string daemonAddress;
    int daemonPort;

    DaemonClientResponse sendMessage(std::string message);
};

#endif //SIMPLE_P2P_DAEMONCLIENT_H
