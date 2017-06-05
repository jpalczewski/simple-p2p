#include <iostream>
#include <thread>
#include <unordered_map>

#include "CryptoUtils.h"
#include "ConfigHandler.h"

#include "Network/UdpListener.h"
#include "Resources/Resource.h"

#include "Resources/ResourceManager.h"
#include "Commands/UserCommandsHandler.h"
#include "Dispatcher.h"


#include "Crypto/AuthorKey.h"

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
	if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " PATH_TO_DIR (containing config.ini download/ share/ and keys/" << std::endl;
		
        std::cout << "\te.g. ./simple-p2p-daemon /simple-p2p/config/docker/" << std::endl;
        return 0;
    }

    ConfigHandler *config;
    config = ConfigHandler::getInstance();
	config->readDirectory(argv[1]);
	
    //std::cout << ConfigHandler::getInstance()->get("daemon.port") << std::endl;
	
    initOpenSsl();

	int serverPort = std::stoi(config->get("network.server_port"));
    int targetPort = std::stoi(config->get("network.client_port"));
	
    AuthorKey authorKey(config->get("keys.dir")+"rsa_public.pem", config->get("keys.dir")+"rsa_private.pem");

    authorKey.generateKey(1024);

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