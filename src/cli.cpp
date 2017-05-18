#include <iostream>
#include "Network/Socket.h"
#include "Dispatcher.h"
#include <thread>
#include <unistd.h>

#include "CryptoUtils.h"
#include "examples.h"
#include "UdpListener.h"

void serverFunc(int port)
{
    UdpListener listener(port);
    listener.start();
}

void clientFunc(int serverPort)
{
    Socket socket(Socket::Domain::Ip4, Socket::Type::Udp);
//    socket.connect("localhost", serverPort, 0);
    socket.enableBroadcast();
    std::cout << "Connected to server.\n";
    char buffer[1024];
    while (true)
    {
        std::cout << "--- Sending broadcast." << std::endl;
        char message[] = {0x00};
        socket.writeTo(message, sizeof(message), "127.255.255.255", serverPort);
        sleep(3);
    }
    socket.close();
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
    clientFunc(clientPort);
    serverThread.join();
    return 0;
}