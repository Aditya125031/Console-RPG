#include "../include/goblin.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream> 
#include <cstdlib> 
#include <vector>  
#include <string> 

Goblin::Goblin(): Enemy("goblin",50,8) 
{

    dialogue.push_back("Shiny things for Grug!");
    dialogue.push_back("Me smash you!");
    dialogue.push_back("*screech*");
}
Goblin::Goblin(Player& player)
    : Enemy("Goblin", 50, 8) 
{
    std::string playerType = player.get_type_string();
    dropLoot.push_back(make_shared<Health_Potion>()); 
    dialogue.push_back("Shiny things for Grug!");
    dialogue.push_back("Me smash you!");
    dialogue.push_back("*screech*");
}
void Goblin::triggerDialogue() const 
{
    if (!dialogue.empty()) {
        std::cout << get_name() << " screeches: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
void Goblin::specialAbility(Character& target) 
{
    // Do nothing. 
    // This satisfies the compiler.
}