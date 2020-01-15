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
int Inventory::getSubId() const {
    return subId;
}
int Inventory::getReceiptId() const {
    return receiptId;
}
void Inventory::increaseReceipt() {
    receiptId++;
}
void Inventory::increaseSubId() {
    subId++;
}
const std::map<std::string,int> &Inventory::getGenreToSubId() const {
    return genreToSubId;
}
const std::map<std::string, std::vector<std::string>> &Inventory::getGenreToBooks() const {
    return genreToBooks;
}




