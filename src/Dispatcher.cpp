//
// Created by kamil on 05/05/17.
//

#include <thread>
#include "Dispatcher.h"
#include <iostream>

Dispatcher::Dispatcher(int listenPort, int maximumConnections) : socket(Socket::Domain::Ip4, Socket::Type::Tcp)
{
    socket.bind(listenPort);
    socket.listen(maximumConnections);
}

void Dispatcher::start()
{
    while (true)
    {
        try
        {
            Socket newSocket = socket.accept();
            std::thread thread([&](Socket socket) -> void
                               { handler.handle(std::move(socket)); }, std::move(newSocket));
            thread.detach();
        }
        catch (const std::exception& e)
        {
            std::cout << "Accepting new connection in dispatcher failed. Reason: " << e.what();
        }
    }
}