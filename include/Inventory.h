//
// Created by arik on 14.1.2020.
//

#ifndef CLIENT_INVENTORY_H
#define CLIENT_INVENTORY_H
class Inventory{
public:
    static Inventory& getInstance();
private:
    Inventory();
};
#endif //CLIENT_INVENTORY_H
