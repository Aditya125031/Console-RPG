#include "../include/orc_raider.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

OrcRaider::OrcRaider()
    : Enemy("Orc Raider", 160, 16) 
{
    dialogue.push_back("For the Horde!");
    dialogue.push_back("Blood and thunder!");
    dialogue.push_back("I will crush you!");
}
OrcRaider::OrcRaider(Player& player)
    : Enemy("Orc Raider", 160, 16) 
{
     std::string playerType = player.get_type_string();

    if (playerType == "Swordsman") 
    {
        dropLoot.push_back(make_shared<Runeforged_Armor>());
        dropLoot.push_back(make_shared<Angel_Blessings>());
        dropLoot.push_back(make_shared<Dragon_Breath>());
    } else if (playerType == "Archer") 
    {
        dropLoot.push_back(make_shared<Lunar_Veil>());
        dropLoot.push_back(make_shared<Angel_Blessings>());
        dropLoot.push_back(make_shared<Dragon_Breath>());
    } else if (playerType == "Mage") 
    {
        dropLoot.push_back(make_shared<Void_Cloak>()); 
        dropLoot.push_back(make_shared<Angel_Blessings>());
        dropLoot.push_back(make_shared<Dragon_Breath>());
    }
    dialogue.push_back("For the Horde!");
    dialogue.push_back("Blood and thunder!");
    dialogue.push_back("I will crush you!");
}
void OrcRaider::triggerDialogue() const 
{
    if (!dialogue.empty()) {
        std::cout << get_name() << " bellows: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
