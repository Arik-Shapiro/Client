//
// Created by arik on 13.1.2020.
//

#ifndef CLIENT_MESSAGE_H
#define CLIENT_MESSAGE_H


#include <string>
#include <map>

class Message {
public:
    Message(std::string);

private:
    std::string command;
    std::map<std::string, std::string> headers;
    std::string body;
public:
    const std::string &getCommand() const;
    std::string toString();
    const std::map<std::string, std::string> &getHeaders() const;
    const std::string &getBody() const;
};
#endif //CLIENT_MESSAGE_H
