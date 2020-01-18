//
// Created by arik on 14.1.2020.
//

#include "../include/ClientProtocol.h"
#include <iostream>
#include <algorithm>

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
    if(command == "ERROR")
    {
        return acceptError(message);
    }
    return nullptr;
}
Message *ClientProtocol::acceptError(Message &message) {
    std::cout << message.getBody() << std::endl; // print all error recieved from server.
    return nullptr;
}
Message *ClientProtocol::acceptMessage(Message &message) {
    const auto& headers = message.getHeaders();
    auto itCommand = headers.find("function");
    if(itCommand != headers.end()) {
        if(itCommand->second == "printForMe")
        {
            auto name = headers.find("name");
            if(name->second == myName)
            {
                std::string temp = message.getBody();
                temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
                replace( temp.begin(), temp.end(), ';', ':' );
                std::cout << temp << std::endl;
                return nullptr;
            }
            return nullptr;
        }
        if (itCommand->second == "printInv") {
            if(inventory.getGenreToBooks().size() == 0) return nullptr;
            std::string inv = inventory.printInv();
            auto dest = headers.find("destination");
            auto name = headers.find("name");
            std::string message = "SEND\ndestination:" + dest->second + "\n" +"function:printForMe\n" +"name:"+name->second+"\n" + this->myName + ";" + inv + "\n";
            return new Message(message);
        }
        if(itCommand->second == "borrow")
        {
            auto name = headers.find("name");
            auto bookName = headers.find("bookName");
            std::string temp = message.getBody();
            temp.erase(std::remove(temp.begin(),temp.end(),'\n'),temp.end());
            std::cout << temp << std::endl;
            if(this->myName != name->second) // i dont want to borrow from my self.
            {
                std::map<std::string, std::vector<std::string>> &books = inventory.getGenreToBooks();
                auto dest = headers.find("destination");
                auto bookExists = books.find(dest->second);
                if(bookExists != books.end()) {// genre exists
                    std::vector<std::string> book = bookExists->second;
                    for(unsigned int i = 0;i<book.size();i++){
                        if(book[i] == bookName->second){//Im the borrower of the book and I have the book.
                            std::string message = "SEND\ndestination:" + dest->second + "\n" +
                                    "function:hasBook\n" + "bookBorrower:"+myName+"\n"+
                                    "bookRec:" + name->second + "\n" + "bookName:"+ bookName->second + "\n" +
                                    myName+" has " + bookName->second + "\n";
                            return new Message(message); //sending a msg that I have the book.
                        }
                    }
                }
            }
        }
        if(itCommand->second == "hasBook")
        {
            auto bookBorrower = headers.find("bookBorrower");
            auto bookRec = headers.find("bookRec");
            auto bookName = headers.find("bookName");
            auto dest = headers.find("destination");
            // Someone has the book I want, checking that I dont already have the book.
            if(bookRec->second == myName && !inventory.hasBook(bookName->second,dest->second)){
                std::string temp = message.getBody();
                temp.erase(std::remove(temp.begin(),temp.end(),'\n'),temp.end());
                std::cout << temp << std::endl;
                inventory.addBook(bookName->second,dest->second);//taking the book
                inventory.bookBorrow(bookName->second,bookBorrower->second);//updating who borrowed me the book
                std::string message = "SEND\ndestination:" + dest->second + "\n" +
                                      "function:tookBook\n" + "bookBorrower:"+bookBorrower->second+"\n"+
                                      + "bookName:"+ bookName->second + "\n" +
                                      +"Taking " + bookName->second + " from " + bookBorrower->second +"\n";
                return new Message(message);//letting the borrower know im taking his book.
            }
        }
        if(itCommand->second == "tookBook")
        {
            std::string temp = message.getBody();
            temp.erase(std::remove(temp.begin(),temp.end(),'\n'),temp.end());
            std::cout << temp << std::endl;
            auto bookBorrower = headers.find("bookBorrower");
            auto bookName = headers.find("bookName");
            auto dest = headers.find("destination");
            //I offered to borrower a book, checking if i am the borrower.
            if((bookBorrower->second == myName )&& inventory.hasBook(bookName->second,dest->second))//i am the borrower
            {
                inventory.removeBook(bookName->second,dest->second);//removing the book from my inventory.
                return nullptr;
            }
        }
        if(itCommand->second == "return")
        {
            std::string temp = message.getBody();
            temp.erase(std::remove(temp.begin(),temp.end(),'\n'),temp.end());
            std::cout << temp << std::endl;
            auto bookBorrower = headers.find("borrowerName");
            if(bookBorrower->second == myName)//Im the original owner of the book, taking it back.
            {
                auto bookName = headers.find("bookName");
                auto dest = headers.find("destination");
                inventory.addBook(bookName->second,dest->second);//adding the book back to my inventory.
                return nullptr;
            }
        }
        return nullptr;
    }
    else {//printing regular messages.
        std::string temp = message.getBody();
        temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
        replace( temp.begin(), temp.end(), ';', ':' );
        std::cout << temp << std::endl;
        return nullptr;
    }
}
bool ClientProtocol::ShouldTerminate() const {
    return shouldTerminate;
}
void ClientProtocol::setShouldTerminate(bool shouldTerminate) {
    ClientProtocol::shouldTerminate = shouldTerminate;
}
ClientProtocol::ClientProtocol() : inventory(Inventory::getInstance()), myName(), shouldTerminate(false) {
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
            std::cout <<"Logout successful" <<std::endl;
            //shouldTerminate = true; continue receiving messages.
            inventory.clearData();// clear all the maps for the next user to login.
            return new Message("DELETE");
        }
    }
    return nullptr;
}
std::string ClientProtocol::processStatus(std::string &dest,std::string &name)
{
    std::string stompMessage =
            "SEND\nfunction:printInv\ndestination:" + dest +"\nname:" +name + '\n' +"book status"+ "\n" ;
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
                    "SEND\ndestination:" + dest
                    +'\n' + name + " has added the book " + bookName +'\n';
            return stompMessage;
        }
        return "";
    }
    books[dest].push_back(bookName);
    std::string stompMessage =
            "SEND\ndestination:" + dest
            +'\n' + name + " has added the book " + bookName +'\n';
    return stompMessage;
}

std::string ClientProtocol::processBorrow(std::string &dest, std::string &bookName, std::string &name) {
    std::string stompMessage =
            "SEND\ndestination:" + dest + "\nfunction:borrow" +"\nname:" +name + '\n'
            +"bookName:" + bookName + "\n" + name +" wish to borrow " + bookName + "\n";
    return stompMessage;
}
std::string ClientProtocol::processReturn(std::string &dest, std::string &bookName, std::string &name) {
    if(inventory.hasBook(bookName,dest)){
        inventory.removeBook(bookName,dest);
    } else return "";//the book isnt even in my inventory.
    std::map<std::string, std::vector<std::string>> &b = inventory.getBookToBorrowers();
    auto itBorrower = b.find(bookName);
    std::string borrowerName;
    if(itBorrower != b.end()){
        std::vector<std::string> &borrowers = itBorrower->second;
        if(!borrowers.empty()){
            borrowerName = borrowers[0];
            borrowers.erase(borrowers.begin());
        }
        else
            return "";//no borrowers to that book
    }
    else
        return "";//the book isnt borrowed from anyone
    std::string stompMessage =
            "SEND\ndestination:" + dest + "\nfunction:return" + "\n"
            +"bookName:" + bookName + "\n" "borrowerName:" + borrowerName +"\n" +
            "Returning " + bookName + " to " + borrowerName + "\n";
    return stompMessage;
}

void ClientProtocol::setMyName(const std::string &myName) {
    ClientProtocol::myName = myName;
}
