//
// Created by kamil on 02/05/17.
//

#include "Socket.h"

#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>
#include <cstring>
#include <iostream>

Socket::Socket(Socket::Domain domain, Socket::Type type)
{
    socketDescriptor = ::socket(static_cast<int>(domain), static_cast<int>(type), 0);
    if (socketDescriptor < 0)
    {
        throw std::runtime_error("Error during creating socket");
    }
    this->domain = domain;
    this->type = type;
}

Socket::Socket(int socketDescriptor, Socket::Domain domain, Socket::Type type)
{
    this->socketDescriptor = socketDescriptor;
    this->domain = domain;
    this->type = type;
}

Socket::~Socket()
{
//    close();
}

Socket::Socket(Socket&& other): socketDescriptor(other.socketDescriptor)
{
    other.socketDescriptor = -1;
    domain = other.domain;
    type = other.type;
}

Socket& Socket::operator=(Socket &&other)
{
    socketDescriptor = other.socketDescriptor;
    other.socketDescriptor = -1;
    domain = other.domain;
    type = other.type;
    return *this;
}

void Socket::bind(int port)
{
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = static_cast<int>(domain);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (::bind(socketDescriptor, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        std:: cout << errno << std::endl;
        throw std::runtime_error("Error on binding.");
    }
}

void Socket::connect(std::string host, int port)
{
    connect(host, port, 0);
}

void Socket::connect(std::string host, int port, int timeout)
{
    struct sockaddr_in serv_addr;
    struct hostent* server = gethostbyname(host.c_str());

    if (server == NULL)
    {
        throw std::runtime_error("Error: no such host");
    }

    serv_addr.sin_family = AF_INET;
    memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (::connect(socketDescriptor, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        throw std::runtime_error("Error: cannot connect.");
    }
}

void Socket::listen(int maxQueueLength)
{
    ::listen(socketDescriptor, maxQueueLength);
}

Socket Socket::accept()
{
    unsigned clilen;
    struct sockaddr_in cli_addr;
    int newSocket = ::accept(socketDescriptor, (struct sockaddr *)&cli_addr, &clilen);

    if (newSocket < 0)
    {
        std::cout << errno << std::endl;
        throw std::runtime_error("Error on accept.");
    }

    Domain domain = static_cast<Domain>(cli_addr.sin_family);
    Type type = getType(newSocket);

    return Socket(newSocket, domain, type);
}

Socket::Type Socket::getType(int socket) const
{
    int type;
    socklen_t length = sizeof(type);

    if (::getsockopt(socket, SOL_SOCKET, SO_DOMAIN, &type, &length) < 0)
        throw std::runtime_error("Cannot get socket type");

    return static_cast<Type>(type);
}

int Socket::read(char* output, int length)
{
    int bytesRead = ::read(socketDescriptor, output, length);

    if (bytesRead < 0)
    {
        std::cout << errno << std::endl;
        throw std::runtime_error("Error during reading from socket");
    }

    return bytesRead;
}

int Socket::write(const char* input, int length)
{
    int bytesSent = ::write(socketDescriptor, input, length);

    if (bytesSent < 0)
    {
        throw std::runtime_error("Error during writing to socket");
    }
    return bytesSent;
}

int Socket::close()
{
    if (socketDescriptor < 0)
        return 1;
    return ::close(socketDescriptor);
}
