//
// Created by arik on 14.1.2020.
//

#include "../include/Inventory.h"
#include <map>

Inventory::Inventory() : receiptIdToCommand(), genreToBooks(), bookToBorrowers(), genreToSubId() {
}

Inventory &Inventory::getInstance() {
    static Inventory inventory;
    return inventory;
}

std::map<int, std::string> &Inventory::getReceiptIdToCommand() {
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

std::map<std::string, int> &Inventory::getGenreToSubId() {
    return genreToSubId;
}

std::map<std::string, std::vector<std::string>> &Inventory::getGenreToBooks() {
    return genreToBooks;
}

std::map<std::string, std::vector<std::string>> &Inventory::getBookToBorrowers() {
    return bookToBorrowers;
}

std::string Inventory::printInv() {
    std::string listOfBooks;
    for (auto it = getGenreToBooks().begin(); it != getGenreToBooks().end(); ++it) {
        std::vector<std::string> books = it->second;
        for (unsigned int i = 0; i < books.size(); i++) {
            listOfBooks += books[i] + ",";
        }
    }
    return listOfBooks.substr(0, listOfBooks.size() - 1);
}

bool Inventory::hasBook(std::string bookName, std::string dest) {
    auto genre = getGenreToBooks().find(dest);
    if (genre != getGenreToBooks().end()) {
        std::vector<std::string> b = genre->second;
        for (unsigned int i = 0; i < b.size(); i++) {
            if (b[i] == bookName)
                return true;
        }
    }
    return false;
}

void Inventory::addBook(std::string bookName, std::string dest) {
    auto genre = getGenreToBooks().find(dest);
    if (genre != getGenreToBooks().end()) {
        getGenreToBooks()[dest].push_back(bookName);
    } else
        getGenreToBooks()[dest].push_back(bookName);
}

void Inventory::clearData() {
    receiptIdToCommand.clear();
    genreToBooks.clear();
    bookToBorrowers.clear();
    genreToSubId.clear();
    subId = 0;
    receiptId = 0;
}

void Inventory::removeBook(std::string bookName, std::string dest) {
    auto genre = getGenreToBooks().find(dest);
    if (genre != getGenreToBooks().end()) {
        std::vector<std::string> &vec = genre->second;
        for (unsigned int i = 0; i < vec.size(); i++) {
            if (vec[i] == bookName)
                vec.erase(vec.begin() + i);
        }
    }
}

void Inventory::bookBorrow(std::string bookName, std::string borrowerName) {
    auto genre = bookToBorrowers.find(bookName);
    if (genre != bookToBorrowers.end()) {
        genre->second.push_back(borrowerName);
    } else
        bookToBorrowers[bookName].push_back(borrowerName);
}


