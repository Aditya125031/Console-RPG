#include "war_chief.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

GoblinWarChief::GoblinWarChief()
    // Mini-boss stats
    : Enemy("Goblin War Chief", 150, 14) 
{
    dialogue.push_back("For the Boss! Attack!");
    dialogue.push_back("You'll make a fine trophy!");
    dialogue.push_back("More guts! More glory!");
}
std::vector<Item> GoblinWarChief::getLoot(const Player& player) const
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
void GoblinWarChief::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " barks: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
