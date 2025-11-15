#include "../include/giant.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

GiantGoblin::GiantGoblin()
    : Enemy("Giant Goblin", 180, 15) 
{
   
}
GiantGoblin::GiantGoblin(Player& player)
    : Enemy("Giant Goblin", 180, 15) 
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
    dialogue.push_back("Roarrr!");
    dialogue.push_back("Grrrr... Smash!");
    dialogue.push_back("You puny human!");
}

void GiantGoblin::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " roars: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}