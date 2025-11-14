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
InfernalImp::InfernalImp(Player& player)
    : Enemy("Infernal Imp", 60, 18) 
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
    dialogue.push_back("Burn, fleshling, burn!");
    dialogue.push_back("The Black Pit awaits!");
    dialogue.push_back("*giggles manically*");
}
void InfernalImp::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " hisses: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
