//
// Created by arik on 14.1.2020.
//

#ifndef CLIENT_INVENTORY_H
#define CLIENT_INVENTORY_H

#include <map>
#include <vector>

class Inventory{
public:
    static Inventory& getInstance();
    const std::map<int, std::string> &getReceiptIdToCommand() const;
    const std::map<std::string, std::vector<std::string>> &getGenreToBooks() const;
    const std::map<std::string, int> &getGenreToSubId() const;
    int getSubId() const;
    void increaseSubId();
    int getReceiptId() const;
    void increaseReceipt();

private:
    std::map<int, std::string> receiptIdToCommand;
    std::map<std::string, std::vector<std::string>> genreToBooks;
    std::map<std::string, int> genreToSubId;
    int subId = 0;
    int receiptId = 0;
private:
    Inventory();
};
#endif //CLIENT_INVENTORY_H
