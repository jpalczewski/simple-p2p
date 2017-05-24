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

void clientFunc(int serverPort)
{
    while (true)
    {
        int a;
        std::cin >> a;
        if (a == 0)
        {
            std::string publicKey = "-----BEGIN RSA PUBLIC KEY-----\n"
                    "MIGJAoGBANLVDLEKgzjGBqiNPTqRqPQynEbbNZbY2T2K+Fn3YMX8hbZ8QLn48CZI\n"
                    "n4mKDQSVND1qxBCeRBxYeaavwyZ8LCqVYPCXZwkRH0QlyReQOCxyw7duEIl/F7tc\n"
                    "N3/34qht2DK8lOp9IQS7sY9L5Oy73JW4jQUMcPUUg52bq7pzccWFAgMBAAE=\n"
                    "-----END RSA PUBLIC KEY-----\n";
            int size = 55;
            const char *testHash = "0123456789012345";
            unsigned char *hashBytes = (unsigned char *) testHash;
            std::vector<unsigned char> hash(testHash, testHash + 16);
            std::vector<unsigned char> sign(128, 0x55);
            Resource resource("name1", size, hash, sign);

            Socket socket(Socket::Domain::Ip4, Socket::Type::Udp);
            socket.enableBroadcast();
            std::cout << "Connected to server.\n";
            std::unordered_map<std::string, std::vector<Resource>> map;
            map[publicKey] = std::vector<Resource>{resource};
            BroadcastMessage message(map);
            const auto bytes = message.toByteStream();
            std::cout << "--- Sending broadcast." << std::endl;
            socket.writeTo((const char *) &bytes[0], bytes.size(), "127.255.255.255", serverPort);
            std::cout << std::to_string(bytes.size()) << "bytes sent." << std::endl;
            sleep(3);
            socket.close();
        }
        else
        {
            const auto resources = resourceManager.getNetworkResources();
            for (const auto &keyRes : resources)
            {
                std::cout << "key: " << std::endl;
                std::cout << keyRes.first << std::endl;
                for (const auto &res : keyRes.second)
                {
                    std::cout << "name: " << std::endl;
                    std::cout << res.first.getName() << std::endl;
                    std::cout << "size: " << std::endl;
                    std::cout << res.first.getSize() << std::endl;
                    for (const auto& ip : res.second.getSeeders())
                        std::cout << "ip: " << ip.address << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }
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