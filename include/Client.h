//
// Created by arik on 13.1.2020.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <string>
#include <queue>
#include "connectionHandler.h"
#include "BlockingQueue.h"
#include "Transmitter.h"
#include <thread>
class Client {
public:
    Client();
private:
    void start();
    void readNextLine();
    ConnectionHandler *handler;
    bool logged_in;
    std::string processUserCommand(std::string &userCommand);
    Transmitter transmitter;
    ClientProtocol protocol;
};


#endif //CLIENT_CLIENT_H
