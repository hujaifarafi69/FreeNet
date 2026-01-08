#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

int connect_to_server(const std::string& ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Failed to connect to server\n";
        return -1;
    }
    return sock;
}
