#include "../include/imp.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib> // For rand()

Imp::Imp()
    // Call base Enemy constructor: Enemy(name, health, attackPower)
    : Enemy("Imp", 35, 10) 
{
    // Add unique dialogue lines
    dialogue.push_back("*cackles*");
    dialogue.push_back("Can't catch me!");
    dialogue.push_back("Hehehe!");
}
std::vector<Item> Imp::getLoot(const Player& player) const
{
    std::vector<Item> allLoot = Enemy::getLoot(player);

    std::string playerType = player.get_type_string();

    if (playerType == "Swordsman") 
    {
        allLoot.push_back(Shinketsu_Sword()); 
    } else if (playerType == "Archer") 
    {
        allLoot.push_back(Void_Embrace()); 
    } else if (playerType == "Mage") 
    {
        allLoot.push_back(Elder_Wand()); 
    }
    return allLoot;
}
void Imp::triggerDialogue() const 
{
    if (!dialogue.empty()) 
    {
        std::cout << get_name() << " shrieks: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}