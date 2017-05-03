#include <iostream>
#include "Socket.h"
#include <thread>
#include <unistd.h>

void serverFunc()
{
    Socket socket(Socket::Domain::Ip4, Socket::Type::Tcp);
    socket.bind(4567);
    socket.listen(10);
    Socket newConnection = socket.accept();
    char buffer[1024];
    newConnection.read(buffer, 1024);
    std::cout << "Received: " << buffer << std::endl;
    socket.close();
}

void clientFunc()
{
    Socket socket(Socket::Domain::Ip4, Socket::Type::Tcp);
    socket.connect("localhost", 4567, 0);
    std::string message = "Hello.\n";
    socket.write(message.c_str(), message.length() );
    std::cout << "Message sent" << std::endl;
    socket.close();
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    std::thread serverThread(serverFunc);
    sleep(1);
    clientFunc();
    serverThread.join();
    return 0;
}