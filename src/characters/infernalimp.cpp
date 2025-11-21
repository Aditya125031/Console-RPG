#include "../include/infernalimp.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

InfernalImp::InfernalImp()
    : Enemy("Infernal Imp", 160, 20) 
{
    dialogue.push_back("Burn, fleshling, burn!");
    dialogue.push_back("The Black Pit awaits!");
    dialogue.push_back("*giggles manically*");
}
InfernalImp::InfernalImp(Player& player)
    : Enemy("Infernal Imp", 160, 18) 
{
     std::string playerType = player.get_type_string();

    if (playerType == "Swordsman") 
    {
        dropLoot.push_back(make_shared<Soul_Reaper>());
    } else if (playerType == "Archer") 
    {
        dropLoot.push_back(make_shared<Eclipse_Striker>());
    } else if (playerType == "Mage") 
    {
        dropLoot.push_back(make_shared<Orb_of_Avarice>()); 
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
