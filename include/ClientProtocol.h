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
private:
    Message* acceptConnected(Message &message);
    Message* acceptReceipt(Message &message);
    bool shouldTerminate;
    Inventory inventory;
};
#endif //CLIENT_CLIENTPROTOCOL_H
