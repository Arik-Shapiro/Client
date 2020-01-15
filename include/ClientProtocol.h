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
    std::string processUserCommand(std::string &command);
    Message* processServerMessage(Message &message);
    bool ShouldTerminate() const;
    void setShouldTerminate(bool shouldTerminate);
    std::string processJoin(std::string &dest);
    std::string processExit(std::string &dest);
    std::string processAdd(std::string &dest, std::string &bookName, std::string &name);
    std::string processLogout();
    std::string processStatus(std::string &);
private:
    Message* acceptConnected(Message &message);
    Message* acceptReceipt(Message &message);
    bool shouldTerminate;
    Inventory inventory;
    Message *acceptMessage(Message &message);
};
#endif //CLIENT_CLIENTPROTOCOL_H
