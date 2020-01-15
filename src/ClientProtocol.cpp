//
// Created by arik on 14.1.2020.
//

#include <include/ClientProtocol.h>
#include <iostream>

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

