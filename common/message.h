#pragma once
#include <string>
#include <ctime>

struct Message {
    std::string sender;
    std::string receiver;
    std::string content;
    std::time_t timestamp;

    std::string serialize() const {
        return sender + "|" + receiver + "|" + std::to_string(timestamp) + "|" + content;
    }

    static Message deserialize(const std::string& str) {
        Message msg;
        size_t pos1 = str.find("|");
        size_t pos2 = str.find("|", pos1 + 1);
        size_t pos3 = str.find("|", pos2 + 1);

        msg.sender = str.substr(0, pos1);
        msg.receiver = str.substr(pos1 + 1, pos2 - pos1 - 1);
        msg.timestamp = std::stoll(str.substr(pos2 + 1, pos3 - pos2 - 1));
        msg.content = str.substr(pos3 + 1);
        return msg;
    }
};
