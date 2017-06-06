//
// Created by kamil on 02/05/17.
//

#ifndef PROJECT_SOCKET_H
#define PROJECT_SOCKET_H

#include <string>
#include <sys/socket.h>
#include "IpAddress.h"

class Socket
{
public:
    enum class Domain {Local = AF_UNIX, Ip4 = AF_INET};
    enum class Type {Tcp = SOCK_STREAM, Udp = SOCK_DGRAM};

    Socket(Domain domain, Type type);
    ~Socket();

    Socket(Socket&& other);
    Socket& operator=(Socket&& other);

    void bind(int port);
    void connect(std::string host, int port);
    void connect(std::string host, int port, int timeout);
    void connectByIp(std::string hostIp, int port);
    void listen(int maxQueueLength);
    Socket accept();
    int read(unsigned char* output, int length);
    int readFrom(unsigned char* output, int length, std::string& receiveAddress);
    int readFrom(unsigned char* output, int length, IpAddress& receiveAddress);
    int write(const unsigned char* input, int length);
    int writeTo(const unsigned char *input, int length, std::string address, int port);
    int close();
    void enableBroadcast();
    void setTimeout(int second);

private:
    int socketDescriptor;
    Domain domain;
    Type type;

    Socket(int socketDescriptor, Socket::Domain domain, Socket::Type type);
    Socket::Type getType(int socket) const;
};

#endif //PROJECT_SOCKET_H
