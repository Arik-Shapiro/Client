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
    std::string name;
    boost::split(command, userCommand, boost::is_any_of(" "));
    if (command[0] == "login" && command.size() > 3 && !connectedSocket) {
        return processLoginCommand(command,name);
    }
    if(command[0] == "join" && connectedSocket){
        return processJoinCommand(command);
    }
    if(command[0] == "exit" && connectedSocket){
        return processExitCommand(command);
    }
    if(command[0] == "add" && connectedSocket){
        return processAddCommand(command,userCommand,name);
    }
    if(command[0] == "logout" && connectedSocket){
        return processLogoutCommand();
    }
    if(command[0] == "status" && connectedSocket){
        return processStatusCommand(command);
    }
    return std::__cxx11::string();
}
std::string Client::processStatusCommand(std::vector<std::string> &command)
{
    std::string dest = command[1];
    return protocol.processStatus(dest);
}
std::string Client::processExitCommand(std::vector<std::string> &command){
    std::string destination = command[1];
    return protocol.processExit( destination);
}
std::string Client::processLogoutCommand()
{
    return protocol.processLogout();
}
std::string Client::processAddCommand(std::vector<std::string> &command, std::string userCommand,std::string name){
    std::string destination = command[1];
    std::string bookName = "";
    for(int i = 2 ; i <command.size();i++)
    {
        bookName+=command[i];
    }
    return protocol.processAdd( destination,bookName,name);
}
std::string Client::processJoinCommand(std::vector<std::string> &command)
{
    std::string destination = command[1];
    return protocol.processJoin(destination);
}
std::string Client::processLoginCommand(std::vector<std::string> &command,std::string name){
    std::string address = command[1];
    int delimiter = address.find(':');
    std::string ip = address.substr(0, delimiter);
    std::string port = address.substr(delimiter + 1);
    handler = new ConnectionHandler(ip, stoi(port));
    if (!handler->connect()) {
        std::cerr << "Cannot connect to " << address << std::endl;
        return "";
    }
    connectedSocket = true;
    std::thread transThread(&Transmitter::run, &transmitter);
    transThread.detach();
    name = command[2];
    std::string stompMessage =
            "CONNECT\naccept-version:1.2\nhost:" + ip + "\nlogin:" + command[2] + "\npasscode:" + command[3] + '\n';
    return stompMessage;
}
Client::Client() : connectedSocket(false), handler(nullptr), protocol(), transmitter(*handler, protocol){
    start();
}
std::string Client::processBorrowCommand(std::vector<std::string> &command) {
    return std::__cxx11::string();
}
