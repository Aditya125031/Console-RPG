#include "../include/inventory.hpp"
#include "../extern/pdcurses/curses.h"
#include <algorithm> // For std::min

// --- FULL INCLUDES ---
// We need the full definitions in the .cpp file
#include "../include/items.h"
#include "../include/player.h"
#include "../include/game.h"
using namespace std;

void Inventory::reapplyAllEquipStats(Player& player, Game& world)
{
    // Reset player to base stats first
    player.reset_stats(); 
    
    // Apply stats from equipped items
    if (equippedWeapon) {
        equippedWeapon->weapon_apply_effects(player, world);
    }
    if (equippedArmor) {
        equippedArmor->armor_apply_effects(player, world);
    }
}

// --- Core Functions (addItem, swapWeapon, swapArmor, usePotion) ---
// (These are the same as the previous version)

bool Inventory::addItem(std::shared_ptr<Item> item, int quantity, Player& player, Game& world)
{
    shared_ptr<Weapon> weapon = dynamic_pointer_cast<Weapon>(item);
    if (weapon)
    {
        if (equippedWeapon == nullptr) {
            equippedWeapon = weapon;
            equippedWeapon->equip();
            world.add_log_message("Equipped " + weapon->get_item_name());
            reapplyAllEquipStats(player, world);
            return true;
        }
        else if (inventoryWeapon == nullptr) {
            inventoryWeapon = weapon;
            world.add_log_message("Added " + weapon->get_item_name() + " to inventory.");
            return true;
        }
        else {
            world.add_log_message("Weapon inventory is full!");
            return false;
        }
    }

    // --- 2. Try to cast to Armor ---
    shared_ptr<Armor> armor=dynamic_pointer_cast<Armor>(item);
    if (armor)
    {
        if (equippedArmor == nullptr) {
            equippedArmor = armor;
            equippedArmor->equip();
            world.add_log_message("Equipped " + armor->get_item_name());
            reapplyAllEquipStats(player, world);
            return true;
        }
        else if (inventoryArmor == nullptr) {
            inventoryArmor = armor;
            world.add_log_message("Added " + armor->get_item_name() + " to inventory.");
            return true;
        }
        else {
            world.add_log_message("Armor inventory is full!");
            return false;
        }
    }
 shared_ptr<Usables> potion = dynamic_pointer_cast<Usables>(item);
if (potion)
{
    string name = potion->get_item_name();

    // Check if this potion type is tracked in the max quantity map 'm'
    if (m.count(name) == 0) {
        world.add_log_message("Error: Potion '" + name + "' has no max stack size.");
        // We will add it anyway, but you should add it to your 'm' map
        // in the Inventory.h constructor.
    }
    
    // If this is the first time, store its prototype
    if (potionStorage.count(name) == 0) {
        potionStorage[name].itemPrototype = potion;
    }
    
    // Get current and max quantities
    // We'll default to 5 if not in the map
    int maxQuantity = m.count(name) ? m[name] : 5; 
    int currentQuantity = potionStorage[name].quantity;
    
    if (currentQuantity >= maxQuantity) {
        world.add_log_message(name + " stack is full!");
        return true; // Not an error, just can't add more
    }
    
    // Calculate how many we can actually add
    int roomLeft = maxQuantity - currentQuantity;
    int amountToAdd = std::min(quantity, roomLeft);
    int leftover = quantity - amountToAdd;
    
    // --- THIS IS THE LINE THAT FIXES THE BUG ---
    potionStorage[name].quantity += amountToAdd;
    
    world.add_log_message("Added " + to_string(amountToAdd) + " " + name + ".");
    
    if (leftover > 0) {
        world.add_log_message(name + " stack is now full. " + to_string(leftover) + " " + name + "(s) were not added.");
    }
    
    return true;
}
return false;
}

void Inventory::swapWeapon(Player& player, Game& world)
{
    if (equippedWeapon == nullptr && inventoryWeapon == nullptr) {
        world.add_log_message("No weapons to swap.");
        return;
    }
    world.add_log_message("Swapped weapons.");
    swap(equippedWeapon, inventoryWeapon);
    if (equippedWeapon) equippedWeapon->equip();
    if (inventoryWeapon) inventoryWeapon->unequip();
    reapplyAllEquipStats(player, world);
}

void Inventory::swapArmor(Player& player, Game& world)
{
    if (equippedArmor == nullptr && inventoryArmor == nullptr) {
        world.add_log_message("No armor to swap.");
        return;
    }
    world.add_log_message("Swapped armor.");
    std::swap(equippedArmor, inventoryArmor);
    if (equippedArmor) equippedArmor->equip();
    if (inventoryArmor) inventoryArmor->unequip();
    reapplyAllEquipStats(player, world);
}

void Inventory::usePotion(const std::string& potionName, Player& player, Game& world)
{
    if (potionStorage.count(potionName) == 0 || potionStorage[potionName].quantity <=0) {
        world.add_log_message("No " + potionName + " left.");
        return;
    }
    std::shared_ptr<Usables> potion = potionStorage[potionName].itemPrototype;
    potion->potion_use(player, world);
    potionStorage[potionName].quantity--;
}
void Inventory::dropEquippedWeapon(Game& world, Player& player)
{
    if (equippedWeapon) {
        world.add_log_message("Dropped " + equippedWeapon->get_item_name());
        equippedWeapon->unequip();
        equippedWeapon = nullptr; // Set the public pointer to null
        reapplyAllEquipStats(player, world); // Re-calc stats
    }
}
void Inventory::dropInventoryWeapon(Game& world)
{
    if (inventoryWeapon) {
        world.add_log_message("Dropped " + inventoryWeapon->get_item_name());
        inventoryWeapon = nullptr;
    }
}
void Inventory::dropEquippedArmor(Game& world, Player& player)
{
    if (equippedArmor) {
        world.add_log_message("Dropped " + equippedArmor->get_item_name());
        equippedArmor->unequip();
        equippedArmor = nullptr;
        reapplyAllEquipStats(player, world);
    }
}
void Inventory::dropInventoryArmor(Game& world)
{
    if (inventoryArmor) {
        world.add_log_message("Dropped " + inventoryArmor->get_item_name());
        inventoryArmor = nullptr;
    }
}

void Inventory::dropPotion(const std::string& potionName, Game& world)
{
    if (potionStorage.count(potionName) && potionStorage[potionName].quantity > 0)
    {
        potionStorage[potionName].quantity--;
        world.add_log_message("Dropped 1 " + potionName);
    }
}
std::map<std::string, int> Inventory::getUsablePotions() const
{
    std::map<std::string, int> usable;
    for (const auto& pair : potionStorage) {
        if (pair.second.quantity > 0) {
            usable[pair.first] = pair.second.quantity;
        }
    }
    return usable;
}
std::shared_ptr<Usables> Inventory::removePotionForLoot(const std::string& potionName, Game& world)
{
    // Check if we have any
    if (potionStorage.count(potionName) && potionStorage[potionName].quantity > 0)
    {
        // 1. Decrement the count (just like your void dropPotion)
        potionStorage[potionName].quantity--;
        world.add_log_message("Moved 1 " + potionName + " to loot box.");
        
        // 2. Return the prototype pointer (this is the new part)
        return potionStorage[potionName].itemPrototype;
    }
    // No potion to remove
    return nullptr; 
}