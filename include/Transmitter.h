//
// Created by arik on 13.1.2020.
//

#ifndef CLIENT_TRANSMITTER_H
#define CLIENT_TRANSMITTER_H

#include "BlockingQueue.h"
#include "connectionHandler.h"
#include "ClientProtocol.h"

class Transmitter{
public:
    Transmitter(ConnectionHandler &handler, ClientProtocol &protocol);
    void run();
    void close();
private:
    ConnectionHandler &handler;
    ClientProtocol &protocol;
};
#endif //CLIENT_TRANSMITTER_H