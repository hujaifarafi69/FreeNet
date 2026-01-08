#pragma once
#include <map>
#include <mutex>
#include <string>
#include "../common/message.h"

class Server {
public:
    void start(int port = 9000);
private:
    std::map<int, std::string> clients; // fd -> username
    std::mutex clients_mutex;

    void handle_client(int client_fd);
    void broadcast_to(const Message& msg);
};
