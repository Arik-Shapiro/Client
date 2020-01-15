//
// Created by arik on 14.1.2020.
//

#include <include/Inventory.h>

Inventory::Inventory() = default;
Inventory &Inventory::getInstance() {
    static Inventory inventory;
    return  inventory;
}

const std::map<int, std::string> &Inventory::getReceiptIdToCommand() const {
    return receiptIdToCommand;
}


