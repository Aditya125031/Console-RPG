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
std::vector<Item> OrcRaider::getLoot(const Player& player) const
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
void OrcRaider::triggerDialogue() const 
{
    if (!dialogue.empty()) {
        std::cout << get_name() << " bellows: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
