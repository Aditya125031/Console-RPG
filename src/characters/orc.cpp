#include "../include/orc.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib> 
Orc::Orc()
    : Enemy("Orc",120,18)  
{
    dialogue.push_back("Smash the weakling!");
    dialogue.push_back("Meat's back on the menu!");
    dialogue.push_back("I'll crush your bones!");
}
void Orc::triggerDialogue() const 
{
    std::cout << get_name() << " bellows: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
}
std::vector<Item> Orc::getLoot(const Player& player) const
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
