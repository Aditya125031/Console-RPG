#ifndef TYPE_H
#define TYPE_H
#include<memory>
class Item;
struct InventorySlot
{
    std::shared_ptr<Item> item = nullptr;
    int quantity = 0;
};
#endif