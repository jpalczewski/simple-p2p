#include <iostream>
#include "Network/Socket.h"
#include "Dispatcher.h"
#include <thread>
#include <unistd.h>

#include "CryptoUtils.h"
#include "examples.h"

void serverFunc(int port)
{
    Dispatcher dispatcher(port, 10);
    dispatcher.start();
}

void clientFunc(int serverPort)
{
    Socket socket(Socket::Domain::Ip4, Socket::Type::Tcp);
    socket.connect("localhost", serverPort, 0);
    std::cout << "Connected to server.\n";
    char buffer[1024];
    while (true)
    {
        std::cout << "--- Write your message." << std::endl;
        std::string message;
        std::cin >> message;
        socket.write(message.c_str(), message.length());
        std::cout << "--- Message sent." << std::endl;
        int bytesRead = socket.read(buffer, 1024);
        std::string response(buffer, bytesRead);
        std::cout << "--- Response:" << std::endl;
        std::cout << response << std::endl << std::endl;
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