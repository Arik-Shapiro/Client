//
// Created by arik on 14.1.2020.
//

#ifndef CLIENT_INVENTORY_H
#define CLIENT_INVENTORY_H

#include <map>

class Inventory{
public:
    static Inventory& getInstance();
    const std::map<int, std::string> &getReceiptIdToCommand() const;
private:
    std::map<int, std::string> receiptIdToCommand;
private:
    Inventory();
};
#endif //CLIENT_INVENTORY_H
