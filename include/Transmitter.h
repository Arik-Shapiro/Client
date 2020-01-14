//
// Created by arik on 13.1.2020.
//

#ifndef CLIENT_TRANSMITTER_H
#define CLIENT_TRANSMITTER_H

#include "BlockingQueue.h"
#include "connectionHandler.h"
class Transmitter{
public:
    Transmitter(BlockingQueue<std::string> &messageQueue);

    void run();
private:
    std::string processUserCommand(std::string&);
    BlockingQueue<std::string> &messageQueue;
    ConnectionHandler *handler;
    bool terminate;
public:
    bool isTerminated() const;

private:
    bool logged_in;
public:
    bool isLoggedIn() const;
};
#endif //CLIENT_TRANSMITTER_H