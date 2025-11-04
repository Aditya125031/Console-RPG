#include "../include/giant.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

GiantGoblin::GiantGoblin()
    : Enemy("Giant Goblin", 180, 15) 
{
    dialogue.push_back("Me bigger! Me stronger!");
    dialogue.push_back("CRUSH PUNY THING!");
    dialogue.push_back("WAAAGH!");
}

void GiantGoblin::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " roars: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
std::vector<Item> GiantGoblin::getLoot(const Player& player) const
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
