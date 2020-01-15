//
// Created by arik on 14.1.2020.
//

#include <include/Inventory.h>

Inventory::Inventory() = default;
Inventory &Inventory::getInstance() {
    static Inventory inventory;
    return  inventory;
}
std::map<int, std::string> &Inventory::getReceiptIdToCommand(){
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
std::map<std::string,int> &Inventory::getGenreToSubId() {
    return genreToSubId;
}
std::map<std::string, std::vector<std::string>> &Inventory::getGenreToBooks(){
    return genreToBooks;
}




