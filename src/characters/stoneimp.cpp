#include "stoneimp.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

StoneImp::StoneImp()
    // High health, low attack
    : Enemy("Stone Imp", 80, 5) 
{
    originalAttackPower = attackPower; // Store initial attack power
    dialogue.push_back("*rumble*...");
    dialogue.push_back("Flesh is weak. Stone is eternal.");
    dialogue.push_back("You cannot break me.");
}
std::vector<Item> StoneImp::getLoot(const Player& player) const
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
void StoneImp::triggerDialogue() const {
     if (!dialogue.empty()) {
        std::cout << get_name() << " grinds out: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
