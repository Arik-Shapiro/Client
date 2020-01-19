//
// Created by arik on 13.1.2020.
//

#include <algorithm>
#include "../include/Message.h"

Message::Message(std::string message) : command(), headers(), body() {
    int endLine= message.find('\n');
    command = message.substr(0, endLine);
    message = message.substr(endLine + 1);
    int colonIndex = message.find(':');
    while(colonIndex != -1 && message[0] != '\n'){
        endLine = message.find('\n');
        std::string header = message.substr(0, endLine);
        std::string key = header.substr(0, colonIndex);
        std::string value = header.substr(colonIndex + 1, header.size() - endLine - 1);
        headers[key] = value;
        message = message.substr(endLine + 1);
        colonIndex = message.find(':');
    }
    body = message;
}
const std::string &Message::getCommand() const {
    return command;
}

const std::map<std::string, std::string> &Message::getHeaders() const {
    return headers;
}

const std::string &Message::getBody() const {
    return body;
}

std::string Message::toString() {
    std::string message = this->command + "\n";
    for(auto it=headers.begin(); it != headers.end(); ++it)
    {
        message += it->first + ":" + it->second + "\n";
    }
    if(!body.empty())
        message+= body;
    return message;
}
