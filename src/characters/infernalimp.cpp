#include "../include/infernalimp.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

InfernalImp::InfernalImp()
    : Enemy("Infernal Imp", 60, 18) 
{
    dialogue.push_back("Burn, fleshling, burn!");
    dialogue.push_back("The Black Pit awaits!");
    dialogue.push_back("*giggles manically*");
}
std::vector<Item>InfernalImp::getLoot(const Player& player) const
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
void InfernalImp::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " hisses: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
