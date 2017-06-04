#include <iostream>
#include <thread>
#include <unordered_map>

#include "CryptoUtils.h"
#include "Network/UdpListener.h"
#include "Resources/Resource.h"
#include "Resources/ResourceManager.h"
#include "Commands/UserCommandsHandler.h"
#include "Dispatcher.h"

void serverFunc(int port)
{
    UdpListener listener(port);
    listener.start();
}

void dispatcherFunc(int port)
{
    Dispatcher dispatcher(port, 10);
    dispatcher.start();
}

int main(int argc, char** argv)
{
    initOpenSsl();
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " SERVER_PORT CLIENT_PORT [opt CLIENT_HANDLER_PORT - default 6000]" << std::endl;
        return 0;
    }
    int serverPort = atoi(argv[1]);
    int targetPort = atoi(argv[2]);
    std::thread serverThread(serverFunc, serverPort);
    std::thread dispatcherThread(dispatcherFunc, serverPort);
    std::cout << "Server started." << std::endl;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    int userHandlerPort = argc > 3 ? atoi(argv[3]) : 6000;
    UserCommandsHandler commandsHandler(targetPort, userHandlerPort);
    std::cout << "User commands handler started." << std::endl;
    commandsHandler.handleUserInput();
    serverThread.join();
    dispatcherThread.join();
    return 0;
}