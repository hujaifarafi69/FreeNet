#include "server.h"
#include <iostream>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

void Server::handle_client(int client_fd) {
    char buffer[1024];
    std::string username;

    // First message is username
    int bytes = read(client_fd, buffer, sizeof(buffer));
    if (bytes <= 0) return;
    buffer[bytes] = '\0';
    username = std::string(buffer);

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients[client_fd] = username;
    }

    std::cout << username << " connected.\n";

    while (true) {
        bytes = read(client_fd, buffer, sizeof(buffer));
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        Message msg = Message::deserialize(buffer);

        broadcast_to(msg);
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(client_fd);
    }
    close(client_fd);
    std::cout << username << " disconnected.\n";
}

void Server::broadcast_to(const Message& msg) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto &[fd, user] : clients) {
        if (user == msg.receiver) {
            std::string data = msg.serialize();
            send(fd, data.c_str(), data.size(), 0);
        }
    }
}

void Server::start(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "Server running on port " << port << "\n";

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        std::thread(&Server::handle_client, this, client_fd).detach();
    }

    close(server_fd);
}
