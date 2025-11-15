#include "../include/imp.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib> // For rand()

Imp::Imp()
    // Call base Enemy constructor: Enemy(name, health, attackPower)
    : Enemy("Imp", 35, 10) 
{
    // Add unique dialogue lines
    dialogue.push_back("*cackles*");
    dialogue.push_back("Can't catch me!");
    dialogue.push_back("Hehehe!");
}
Imp::Imp(Player& player)
    : Enemy("Imp", 35, 10) 
{
    std::string playerType = player.get_type_string();
    dropLoot.push_back(make_shared<Mana_Potion>()); 
    dropLoot.push_back(make_shared<Health_Potion>()); 
    // Add unique dialogue lines
    dialogue.push_back("*cackles*");
    dialogue.push_back("Can't catch me!");
    dialogue.push_back("Hehehe!");
}
void Imp::triggerDialogue() const 
{
    if (!dialogue.empty()) 
    {
        std::cout << get_name() << " shrieks: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}