#include <iostream>
#include "Network/Socket.h"
#include <thread>
#include <unistd.h>
#include <unordered_map>

#include "CryptoUtils.h"
#include "Network/UdpListener.h"
#include "Resources/Resource.h"
#include "Messages/BroadcastMessage.h"
#include "Resources/ResourceManager.h"

void serverFunc(int port)
{
    UdpListener listener(port);
    listener.start();
}

int main(int argc, char** argv)
{
    initOpenSsl();
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " SERVER_PORT CLIENT_PORT" << std::endl;
        return 0;
    }
    int serverPort = atoi(argv[1]);
    int clientPort = atoi(argv[2]);
    std::thread serverThread(serverFunc, serverPort);
    std::cout << "Server started. Press any key to connect to other server." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    serverThread.join();
    return 0;
}