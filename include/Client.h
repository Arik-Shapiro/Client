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

    virtual ~Client();

    Client(const Client &aClient);

    Client &operator=(Client &sess);

    void start();

    ConnectionHandler *getHandler() const;

    const ClientProtocol &getProtocol() const;

    const Transmitter &getTransmitter() const;

    std::string *getName() const;

    bool isConnectedSocket() const;

    bool isInputRec() const;

private:
    void readNextLine();

    ConnectionHandler *handler;

    std::string processUserCommand(std::string &userCommand);

    ClientProtocol protocol;
    Transmitter transmitter;
    std::string *name;
    bool connectedSocket;
    bool inputRec;

    std::string processLoginCommand(std::vector<std::string> &command);

    std::string processJoinCommand(std::vector<std::string> &command);

    std::string processExitCommand(std::vector<std::string> &command);

    std::string processAddCommand(std::vector<std::string> &command);

    std::string processBorrowCommand(std::vector<std::string> &command);

    std::string processLogoutCommand();

    std::string processStatusCommand(std::vector<std::string> &command);

    std::string processReturnCommand(std::vector<std::string> &command);
};

#endif //CLIENT_CLIENT_H
