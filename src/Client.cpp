//
// Created by arik on 13.1.2020.
//

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "../include/Client.h"

void Client::start() {
    readNextLine();
}
void Client::readNextLine() {
    while(!protocol.ShouldTerminate()){
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        std::string stompCommand = processUserCommand(line);
        if(!stompCommand.empty())
            handler->sendLine(stompCommand);
    }
    delete(handler);
}
std::string Client::processUserCommand(std::string &userCommand) {
    std::vector<std::string> command;
    boost::split(command, userCommand, boost::is_any_of(" "));
    if (command[0] == "login" && command.size() > 3 && !logged_in) {
        std::string address = command[1];
        int delimiter = address.find(':');
        std::string ip = address.substr(0, delimiter);
        std::string port = address.substr(delimiter + 1);
        handler = new ConnectionHandler(ip, stoi(port));
        if (!handler->connect()) {
            std::cerr << "Cannot connect to " << address << std::endl;
            return "";
        }
        logged_in = true;
        std::thread transThread(&Transmitter::run, &transmitter, std::ref(*handler));
        transThread.detach();
        std::string stompMessage =
                "CONNECT\naccept-version:1.2\nhost:" + ip + "\nlogin:" + command[2] + "\npasscode:" + command[3] + '\n';
        return stompMessage;
    }
    return std::__cxx11::string();
}
Client::Client() : logged_in(false), handler(nullptr), protocol(), transmitter(protocol){
    start();
}
