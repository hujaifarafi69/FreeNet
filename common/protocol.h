#pragma once
#include "../common/message.h"
#include <string>

// Protocol message types
enum class MsgType {
    LOGIN,
    CHAT,
    LOGOUT
};

// Optional: we can extend this later
struct ProtocolMessage {
    MsgType type;
    Message msg;

    std::string serialize() const {
        return std::to_string(static_cast<int>(type)) + "#" + msg.serialize();
    }

    static ProtocolMessage deserialize(const std::string& data) {
        ProtocolMessage pmsg;
        size_t pos = data.find("#");
        int t = std::stoi(data.substr(0, pos));
        pmsg.type = static_cast<MsgType>(t);
        pmsg.msg = Message::deserialize(data.substr(pos + 1));
        return pmsg;
    }
};
