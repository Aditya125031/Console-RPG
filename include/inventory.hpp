#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include"type.hpp"
using namespace std;
// --- FORWARD DECLARATIONS ---
// We only need pointers/references in the header,
// so we don't need the full #include.
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
    // --- Member Variables (All Public) ---

    // Equipment Slots: Rule: Max 1 equipped, Max 1 in the "bag"
    map<string,int> m;
    shared_ptr<Weapon> equippedWeapon = nullptr;
    shared_ptr<Armor>  equippedArmor = nullptr;
    shared_ptr<Weapon> inventoryWeapon = nullptr; // "Bag" slot
    shared_ptr<Armor>  inventoryArmor = nullptr;  // "Bag" slot

    // Potion/Consumable Storage: Maps "Potion Name" -> {prototype, quantity}
    std::map<std::string, PotionSlot> potionStorage;

    // --- Constructor ---
    Inventory() {
        m["Health Potion"]=5;
        m["Angel's Blessings"]=3;
        m["Elixir Of Life"]=2;
        m["Mana Potion"]=5;
        m["Dragon Breath"]=3;
        m["Aether Elixir"]=2;
    } 

    // --- Helper Function ---
    
    /**
     * @brief Resets player stats and applies stats from equipped items.
     * Must be public to be called from Inventory.cpp.
     */
    void reapplyAllEquipStats(Player& player, Game& world);

    // --- Core Functions ---

    /**
     * @brief Tries to add an item. Fills equipped slot first, then bag slot.
     * Fails (returns false) if both slots are full.
     * @return True on success, False if inventory is full.
     */
    bool addItem(std::shared_ptr<Item> item, int quantity, Player& player, Game& world);

    /**
     * @brief Swaps the equipped weapon with the one in the bag.
     */
    void swapWeapon(Player& player, Game& world);

    /**
     * @brief Swaps the equipped armor with the one in the bag.
     */
    void swapArmor(Player& player, Game& world);

    /**
     * @brief Uses a potion from storage by its name.
     */
    void usePotion(const std::string& potionName, Player& player, Game& world);
    
    // --- Drop Functions ---

    /**
     * @brief Drops the equipped weapon.
     */
    void dropEquippedWeapon(Game& world, Player& player);

    /**
     * @brief Drops the weapon from the inventory/bag slot.
     */
    void dropInventoryWeapon(Game& world);

    /**
     * @brief Drops the equipped armor.
     */
    void dropEquippedArmor(Game& world, Player& player);

    /**
     * @brief Drops the armor from the inventory/bag slot.
     */
    void dropInventoryArmor(Game& world);

    /**
     * @brief Drops one potion of the specified type.
     */
    void dropPotion(const std::string& potionName, Game& world);

    // --- Utility Function ---

    /**
     * @brief Gets a list of all potions with quantity > 0.
     * @return A map of Potion Name -> Quantity
     */
    std::map<std::string, int> getUsablePotions() const;
    std::shared_ptr<Usables> removePotionForLoot(const std::string& potionName, Game& world);
};

#endif // INVENTORY_H