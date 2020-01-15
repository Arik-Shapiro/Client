//
// Created by arik on 13.1.2020.
//

#include <include/Transmitter.h>
#include <include/Message.h>
#include <boost/algorithm/string.hpp>

void Transmitter::run() {
    while (!protocol.ShouldTerminate()) {
        std::string answer;
        if (!handler.getLine(answer)) {
            close();
        }
        Message stompAnswer(answer);
        Message *stompMessage = protocol.processServerMessage(stompAnswer);
        std::string stompString = stompAnswer.toString();
        if (!stompMessage)
            if (!handler.sendLine(stompString)) {
                close();
            }
    }
}


Transmitter::Transmitter(ConnectionHandler &handler, ClientProtocol &protocol) : handler(handler),
                                                                                 protocol(protocol) {
}

void Transmitter::close() {
    handler.close();
    protocol.setShouldTerminate(true);
}

