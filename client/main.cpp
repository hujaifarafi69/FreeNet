#include "../common/message.h"
#include <sys/socket.h>  // send()
#include <unistd.h>      // read(), close()
#include <arpa/inet.h>   // sockaddr_in, inet_pton
#include <thread>
#include <iostream>
#include <string>
#include <ctime>

static void receive_messages(int sock) {
    char buffer[1024];
    while (true) {
        int bytes = read(sock, buffer, sizeof(buffer));
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        Message msg = Message::deserialize(buffer);
        std::cout << "[" << msg.sender << "]: " << msg.content << std::endl;
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Failed to connect to server\n";
        return 1;
    }

    std::string username;
    std::cout << "Enter your username: ";
    std::getline(std::cin, username);
    send(sock, username.c_str(), username.size(), 0);

    std::thread(receive_messages, sock).detach();

    std::string receiver, content;
    while (true) {
        std::cout << "Send to: ";
        std::getline(std::cin, receiver);
        std::cout << "Message: ";
        std::getline(std::cin, content);

        if (receiver == "exit" || content == "exit") break;

        Message msg{username, receiver, content, std::time(nullptr)};
        std::string data = msg.serialize();
        send(sock, data.c_str(), data.size(), 0);
    }

    close(sock);
}
