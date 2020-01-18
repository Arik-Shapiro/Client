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
    while(inputRec){
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        if(line == "bye")
        {
            protocol.setShouldTerminate(true);
            std::cout<<"Bye bye...disconnecting"<<std::endl;
            break;
        }
        std::string stompCommand = processUserCommand(line);
        if(!stompCommand.empty())
            handler->sendLine(stompCommand);
    }
}
std::string Client::processUserCommand(std::string &userCommand) {
    std::vector<std::string> command;
    boost::split(command, userCommand, boost::is_any_of(" "));
    if (command[0] == "login" && command.size() > 3 && !connectedSocket) {
        return processLoginCommand(command);
    }
    if(!connectedSocket)
        return "";
    if(command[0] == "join")
        return processJoinCommand(command);
    if(command[0] == "exit")
        return processExitCommand(command);
    if(command[0] == "add")
        return processAddCommand(command);
    if(command[0] == "logout")
        return processLogoutCommand();
    if(command[0] == "status")
        return processStatusCommand(command);
    if(command[0] == "borrow")
        return processBorrowCommand(command);
    if(command[0] == "return")
        return processReturnCommand(command);
    return std::__cxx11::string();
}
std::string Client::processStatusCommand(std::vector<std::string> &command)
{
    std::string dest = command[1];
    return protocol.processStatus(dest,*name);
}
std::string Client::processExitCommand(std::vector<std::string> &command){
    std::string destination = command[1];
    return protocol.processExit( destination);
}
std::string Client::processLogoutCommand()
{
    connectedSocket = false;
    return protocol.processLogout();
}
std::string Client::processAddCommand(std::vector<std::string> &command){
    std::string destination = command[1];
    std::string bookName;
    for(unsigned i = 2 ; i <command.size();i++)
    {
        bookName+=command[i] + " ";
    }
    bookName.resize(bookName.size() -1);
    return protocol.processAdd( destination,bookName, *name);
}
std::string Client::processJoinCommand(std::vector<std::string> &command)
{
    std::string destination = command[1];
    return protocol.processJoin(destination);
}
std::string Client::processLoginCommand(std::vector<std::string> &command){
    std::string address = command[1];
    int delimiter = address.find(':');
    std::string ip = address.substr(0, delimiter);
    std::string port = address.substr(delimiter + 1);
    if(handler != nullptr) delete(handler);
    handler = new ConnectionHandler(ip, stoi(port));
    if (!handler->connect()) {
        std::cerr << "Cannot connect to server " << address << std::endl;
        return "";
    }
    connectedSocket = true;
    protocol.setShouldTerminate(false);
    std::thread transThread(&Transmitter::run, &transmitter, std::ref(*handler));
    transThread.detach();
    name = new std::string(command[2]);
    protocol.setMyName(command[2]);
    std::string stompMessage =
            "CONNECT\naccept-version:1.2\nhost:" + ip + "\nlogin:" + command[2] + "\npasscode:" + command[3] + '\n';
    return stompMessage;
}
Client::Client() : handler(nullptr), protocol(), transmitter(protocol), name(new std::string("")),connectedSocket(false), inputRec(true){
    start();
}
std::string Client::processBorrowCommand(std::vector<std::string> &command) {
    std::string bookName;
    std::string destination = command[1];
    for(unsigned int i = 2 ; i <command.size();i++)
    {
        bookName+=command[i] + " ";
    }
    bookName.resize(bookName.size() -1);
    return protocol.processBorrow(destination,bookName,*name);
}
std::string Client::processReturnCommand(std::vector<std::string> &command) {
    std::string bookName;
    std::string destination = command[1];
    for(unsigned int i = 2 ; i <command.size();i++)
    {
        bookName+=command[i] + " ";
    }
    bookName.resize(bookName.size() -1);
    return protocol.processReturn(destination,bookName,*name);
}

Client::~Client() {
    protocol.setShouldTerminate(true);
    delete(handler);
    delete(name);
}
