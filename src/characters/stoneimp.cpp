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
StoneImp::StoneImp(Player& player)
    // High health, low attack
    : Enemy("Stone Imp", 80, 5) 
{
     std::string playerType = player.get_type_string();

    if (playerType == "Swordsman") 
    {
        dropLoot.push_back(make_shared<Shinketsu_Sword>());
    } else if (playerType == "Archer") 
    {
        dropLoot.push_back(make_shared<Void_Embrace>());
    } else if (playerType == "Mage") 
    {
        dropLoot.push_back(make_shared<Elder_Wand>()); 
    }
    originalAttackPower = attackPower; // Store initial attack power
    dialogue.push_back("*rumble*...");
    dialogue.push_back("Flesh is weak. Stone is eternal.");
    dialogue.push_back("You cannot break me.");
}
void StoneImp::triggerDialogue() const {
     if (!dialogue.empty()) {
        std::cout << get_name() << " grinds out: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
