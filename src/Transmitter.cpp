//
// Created by arik on 13.1.2020.
//

#include "../include/Transmitter.h"
#include "../include/Message.h"

void Transmitter::run(ConnectionHandler &handler) {
    while (!protocol.ShouldTerminate()) {
        std::string answer;
        if (!handler.getLine(answer)) {
            close();
        }
        Message stompAnswer(answer);
        Message *stompMessage = protocol.processServerMessage(stompAnswer);
        if (stompMessage != nullptr) {
            if (stompMessage->getCommand() == "DELETE") {
                delete (stompMessage);
                close();
            }
            else
                {
                std::string stompString = stompMessage->toString();
                    delete(stompMessage);
                if (!handler.sendLine(stompString)) {
                    close();
                    }
                }
            }
    }
    handler.close();
}


Transmitter::Transmitter(ClientProtocol &protocol) : protocol(protocol) {
}

void Transmitter::close() {
    protocol.setShouldTerminate(true);
}

