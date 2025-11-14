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
Orc::Orc(Player& player)
    : Enemy("Orc",120,18)  
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
    dialogue.push_back("Smash the weakling!");
    dialogue.push_back("Meat's back on the menu!");
    dialogue.push_back("I'll crush your bones!");
}
