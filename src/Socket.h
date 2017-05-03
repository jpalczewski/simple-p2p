//
// Created by kamil on 02/05/17.
//

#ifndef PROJECT_SOCKET_H
#define PROJECT_SOCKET_H

#include <string>
#include <sys/socket.h>

class Socket
{
public:
    enum class Domain {Local = AF_UNIX, Ip4 = AF_INET};
    enum class Type {Tcp = SOCK_STREAM, Udp = SOCK_DGRAM};

    Socket() {};
    Socket(Domain domain, Type type);
    ~Socket();

    void bind(int port);
    void connect(std::string host, int port);
    void connect(std::string host, int port, int timeout);
    void listen(int maxQueueLength);
    Socket accept();
    int read(char* output, int length);
    int write(const char* input, int length);
    int close();
private:
    int socketDescriptor;
    Domain domain;
    Type type;

    Socket(int socketDescriptor, Socket::Domain domain, Socket::Type type);
    Socket::Type getType(int socket) const;
};

#endif //PROJECT_SOCKET_H
