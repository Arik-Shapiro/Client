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
    std::map<int, std::string> &getReceiptIdToCommand();
    std::map<std::string, std::vector<std::string>> &getGenreToBooks();
    std::map<std::string, std::vector<std::string>> &getBookToBorrowers();
    std::map<std::string, int> &getGenreToSubId();
    int getSubId() const;
    void increaseSubId();
    int getReceiptId() const;
    void increaseReceipt();
    std::string  printInv();
    bool hasBook(std::string,std::string);
    void addBook(std::string, std::string);
    void removeBook(std::string, std::string);
    void bookBorrow(std::string bookName, std::string borrowerName);
;
private:
    std::map<int, std::string> receiptIdToCommand;
    std::map<std::string, std::vector<std::string>> genreToBooks;
    std::map<std::string, std::vector<std::string>> bookToBorrowers;
    std::map<std::string, int> genreToSubId;
    int subId = 0;
    int receiptId = 0;
private:
    Inventory();
};
#endif //CLIENT_INVENTORY_H
