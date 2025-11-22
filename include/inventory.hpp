#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include"type.hpp"
using namespace std;
class Item;
class Weapon;
class Armor;
class Player;
class Game;
class Usables;
struct PotionSlot
{
    shared_ptr<Usables> itemPrototype = nullptr;
    int quantity = 0;
};

class Inventory
{
public:
    map<string,int> m;
    shared_ptr<Weapon> equippedWeapon = nullptr;
    shared_ptr<Armor>  equippedArmor = nullptr;
    shared_ptr<Weapon> inventoryWeapon = nullptr; 
    shared_ptr<Armor>  inventoryArmor = nullptr;  

    std::map<std::string, PotionSlot> potionStorage;

    Inventory() {
        m["Health Potion"]=5;
        m["Angel's Blessings"]=3;
        m["Elixir Of Life"]=2;
        m["Mana Potion"]=5;
        m["Dragon Breath"]=3;
        m["Aether Elixir"]=2;
    } 

    void reapplyAllEquipStats(Player& player, Game& world);

    bool addItem(std::shared_ptr<Item> item, int quantity, Player& player, Game& world);

    void swapWeapon(Player& player, Game& world);

    void swapArmor(Player& player, Game& world);

    void usePotion(const std::string& potionName, Player& player, Game& world);
    
    void dropEquippedWeapon(Game& world, Player& player);

    void dropInventoryWeapon(Game& world);

    void dropEquippedArmor(Game& world, Player& player);

    void dropInventoryArmor(Game& world);

    void dropPotion(const std::string& potionName, Game& world);

    std::map<std::string, int> getUsablePotions() const;
    
    std::shared_ptr<Usables> removePotionForLoot(const std::string& potionName, Game& world);
};

#endif // INVENTORY_H