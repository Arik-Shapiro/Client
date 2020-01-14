//
// Created by arik on 13.1.2020.
//

#include "../include/Client.h"

void Client::start() {
    std::thread transThread(&Transmitter::run, &transmitter);
    readNextLine();
}
void Client::readNextLine() {
    while(!transmitter.isTerminated()){
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        messageQueue.push(line);
    }
}

Client::Client() : messageQueue(), transmitter(messageQueue){
    start();
}
