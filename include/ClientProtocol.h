//
// Created by arik on 14.1.2020.
//

#ifndef CLIENT_CLIENTPROTOCOL_H
#define CLIENT_CLIENTPROTOCOL_H

#include <string>
#include "Inventory.h"
#include "Message.h"

class ClientProtocol{
public:
    ClientProtocol();
    Message* processServerMessage(Message &message);
    bool ShouldTerminate() const;
    void setShouldTerminate(bool shouldTerminate);
    std::string processJoin(std::string &dest);
    std::string processExit(std::string &dest);
    std::string processAdd(std::string &dest, std::string &bookName, std::string &name);
    std::string processBorrow(std::string &dest, std::string &bookName, std::string &name);
    std::string processLogout();
    std::string processStatus(std::string &,std::string &name);
    std::string processReturn(std::string &dest, std::string &bookName, std::string &name);
    int addLoginError();
    void setMyName(const std::string &myName);
private:
    Message* acceptConnected(Message &message);
    Message* acceptReceipt(Message &message);
    Inventory inventory;
    std::string myName;
    bool shouldTerminate;
public:
    bool isShouldTerminate() const;

private:
    Message *acceptMessage(Message &message);
    Message *acceptError(Message &message);

};
#endif //CLIENT_CLIENTPROTOCOL_H
