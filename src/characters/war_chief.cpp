#include "war_chief.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

GoblinWarChief::GoblinWarChief()
    // Mini-boss stats
    : Enemy("Goblin War Chief", 150, 15) 
{
    dialogue.push_back("For the Boss! Attack!");
    dialogue.push_back("You'll make a fine trophy!");
    dialogue.push_back("More guts! More glory!");
}
GoblinWarChief::GoblinWarChief(Player& player)
    // Mini-boss stats
    : Enemy("Goblin War Chief", 150, 14) 
{
     std::string playerType = player.get_type_string();

    if (playerType == "Swordsman") 
    {
        dropLoot.push_back(make_shared<Shinketsu_Sword>());
        dropLoot.push_back(make_shared<Angel_Blessings>());
        dropLoot.push_back(make_shared<Dragon_Breath>());
    } else if (playerType == "Archer") 
    {
        dropLoot.push_back(make_shared<Void_Embrace>());
        dropLoot.push_back(make_shared<Angel_Blessings>());
        dropLoot.push_back(make_shared<Dragon_Breath>());
    } else if (playerType == "Mage") 
    {
        dropLoot.push_back(make_shared<Elder_Wand>()); 
        dropLoot.push_back(make_shared<Angel_Blessings>());
        dropLoot.push_back(make_shared<Dragon_Breath>());
    }
    dialogue.push_back("For the Boss! Attack!");
    dialogue.push_back("You'll make a fine trophy!");
    dialogue.push_back("More guts! More glory!");
}
void GoblinWarChief::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " barks: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
