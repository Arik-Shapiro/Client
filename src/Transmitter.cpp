//
// Created by arik on 13.1.2020.
//

#include <include/Transmitter.h>
#include <include/Message.h>
#include <boost/algorithm/string.hpp>

void Transmitter::run() {
    while (!terminate) {
        std::string userCommand = messageQueue.pop();
        std::string processedCommand = processUserCommand(userCommand);
        if (logged_in) {
            handler->sendLine(processedCommand);
            std::string answer;
            if (!handler->getLine(answer)) {
                terminate = true;
            } else {
                Message *msg = new Message(answer);
                if(msg->getCommand() == "CONNECTED")
                    std::cout << "User is connected" << std::endl;
            }
        } else {
            std::cerr << "You must log in before attempting to: " << userCommand << std::endl;
        }
    }
}

Transmitter::Transmitter(BlockingQueue<std::string> &messageQueue) : messageQueue(messageQueue), terminate(false),
                                                             logged_in(false), handler(
                nullptr) {
}

std::string Transmitter::processUserCommand(std::string &userCommand) {
    std::vector<std::string> command;
    boost::split(command, userCommand, boost::is_any_of(" "));
    if (command[0] == "login" && command.size() > 3) {
        std::string address = command[1];
        int delimiter = address.find(':');
        std::string ip = address.substr(0, delimiter);
        std::string port = address.substr(delimiter + 1);
        handler = new ConnectionHandler(ip, stoi(port));
        if (!handler->connect())
            std::cerr << "Cannot connect to " << address << std::endl;
        logged_in = true;
        std::string stompMessage =
                "CONNECT\naccept-version:1.2\nhost:" + ip + "\nlogin:" + command[2] + "\npasscode:" + command[3] + '\n';
        return stompMessage;
    }
    return std::__cxx11::string();
}

bool Transmitter::isLoggedIn() const {
    return logged_in;
}

bool Transmitter::isTerminated() const {
    return terminate;
}

