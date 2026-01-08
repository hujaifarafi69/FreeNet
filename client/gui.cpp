#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <ctime>
#include "../common/message.h"
#include <string>

void receive_messages(int sock) {
    char buffer[1024];
    while (true) {
        int bytes = read(sock, buffer, sizeof(buffer));
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        Message msg = Message::deserialize(buffer);
        std::cout << "\n[" << msg.sender << "]: " << msg.content << "\n> ";
    }
}

void start_client_gui(const std::string& username, int sock) {
    std::thread(receive_messages, sock).detach();

    std::string receiver, content;
    while (true) {
        std::cout << "> Send to: ";
        std::getline(std::cin, receiver);
        std::cout << "> Message: ";
        std::getline(std::cin, content);

        if (receiver == "exit" || content == "exit") break;

        Message msg{username, receiver, content, std::time(nullptr)};
        std::string data = msg.serialize();

        // SEND TO SERVER
        send(sock, data.c_str(), data.size(), 0);
    }

    close(sock);
}
