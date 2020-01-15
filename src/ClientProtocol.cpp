//
// Created by arik on 14.1.2020.
//

#include <include/ClientProtocol.h>
#include <iostream>
#include <algorithm>

std::string ClientProtocol::processUserCommand(std::string &command) {
    return nullptr;
}

Message* ClientProtocol::processServerMessage(Message &message) {
    const std::string& command = message.getCommand();
    if(command == "CONNECTED"){
        return acceptConnected(message);
    }
    if(command == "RECEIPT"){
        return acceptReceipt(message);
    }
    if(command == "MESSAGE")
    {
        return acceptMessage(message);
    }
}
Message *ClientProtocol::acceptMessage(Message &message) {
    const auto& headers = message.getHeaders();
    auto itReceipt = headers.find("receipt");
    if(itReceipt != headers.end()) {
        int receiptId = std::stoi(itReceipt->second);
        auto itCommand = inventory.getReceiptIdToCommand().find(receiptId);
        if (itCommand != inventory.getReceiptIdToCommand().end()) {
            if(itCommand->second == "ADD")
            {
                std::string print = Message::stringWithoutReceipt(message);
                std::cout << print;
            }
        }
    }
}
bool ClientProtocol::ShouldTerminate() const {
    return shouldTerminate;
}

void ClientProtocol::setShouldTerminate(bool shouldTerminate) {
    ClientProtocol::shouldTerminate = shouldTerminate;
}

ClientProtocol::ClientProtocol() : inventory(Inventory::getInstance()), shouldTerminate(false) {
}

Message *ClientProtocol::acceptConnected(Message &message) {
    std::cout << "Login successful" << std::endl;
    return nullptr;
}

Message *ClientProtocol::acceptReceipt(Message &message) {
    std::cout << message.getBody() << std::endl;
    const auto& headers = message.getHeaders();
    auto itReceipt = headers.find("receipt-id");
    if(itReceipt != headers.end()) {
        int receiptId = std::stoi(itReceipt->second);
        auto itCommand = inventory.getReceiptIdToCommand().find(receiptId);
        if(itCommand != inventory.getReceiptIdToCommand().end()){
            std::cout << "Logout successful";
            shouldTerminate = true;
        }
    }
    return nullptr;
}
std::string ClientProtocol::processStatus(std::string &dest)
{
    std::map<int,std::string> &rec = inventory.getReceiptIdToCommand();
    rec.insert({inventory.getReceiptId(),"STATUS"});
    std::string stompMessage =
            "SEND\nreceipt:" + std::to_string(inventory.getReceiptId()) + "destination:" + dest +"\n";
    inventory.increaseReceipt();
    return stompMessage;
};
std::string ClientProtocol::processLogout() {
    std::map<int,std::string> &rec = inventory.getReceiptIdToCommand();
    rec.insert({inventory.getReceiptId(),"Logout"});
    std::string stompMessage =
            "DISCONNECT\nreceipt:" + std::to_string(inventory.getReceiptId()) + "\n";
    inventory.increaseReceipt();
    return stompMessage;
}
std::string ClientProtocol::processJoin(std::string &dest){
    std::map<std::string,int> &genreToId = inventory.getGenreToSubId();
    auto itReceipt = genreToId.find(dest);
    if(itReceipt != genreToId.end()) { // user already subscribed to that genre
        std::string stompMessage =
                "SUBSCRIBE\ndestination:" + dest + "\nid:" + std::to_string(-1) + "\nreceipt:" +
                std::to_string(-1) + '\n';
        return stompMessage;
    }
    //user isnt subscrbied to the genre;
    std::string stompMessage =
            "SUBSCRIBE\ndestination:" + dest + "\nid:" + std::to_string(inventory.getSubId()) + "\nreceipt:" +
            std::to_string(inventory.getReceiptId()) + '\n';
        genreToId.insert({dest, inventory.getSubId()});// gave a topic an id
        inventory.increaseReceipt();
        inventory.increaseSubId();
    return stompMessage;
}
std::string ClientProtocol::processExit(std::string &dest)
{
    std::map<std::string,int> &genreToId = inventory.getGenreToSubId();
    auto itReceipt = genreToId.find(dest);
    if(itReceipt != genreToId.end()) { // user is subscribed to that genre
        std::string stompMessage =
                "UNSUBSCRIBE\nid:" + std::to_string(itReceipt->second) + '\n';
        inventory.increaseReceipt();
        genreToId.erase(itReceipt); // removes the genre
        return stompMessage;
    }
    std::string stompMessage ="UNSUBSCRIBE\nid:" + std::to_string(-1) + '\n'; //isnt subscribed and asks to unsubscrib
    return stompMessage;
}
std::string ClientProtocol::processAdd(std::string &dest, std::string &bookName,std::string &name)
{
    std::map<std::string, std::vector<std::string>> &books = inventory.getGenreToBooks();
    auto itReceipt = books.find(dest);
    if(itReceipt != books.end()) {
        std::vector<std::string> &booksOfGenre= itReceipt->second;
        if(!std::count(booksOfGenre.begin(),booksOfGenre.end(),bookName))
            {
            booksOfGenre.push_back(bookName);
                std::string stompMessage =
                        "SEND\ndestination:" + dest + "receipt:" + std::to_string(inventory.getReceiptId())
                        +'\n' + name + "has added the book" + bookName +'\n';
               std::map<int,std::string> &rec = inventory.getReceiptIdToCommand();
               rec.insert({inventory.getReceiptId(),"ADD"});
               inventory.increaseReceipt();
               return stompMessage;
            }
        return "";
    }
    return "";
}


