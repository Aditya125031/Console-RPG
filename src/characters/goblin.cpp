#include "../include/goblin.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream> 
#include <cstdlib> 
#include <vector>  
#include <string> 

Goblin::Goblin(): Enemy("goblin",50,8) 
{
    this->dropLoot.push_back(Health_Potion());
    this->dropLoot.push_back(Mana_Potion());

    dialogue.push_back("Shiny things for Grug!");
    dialogue.push_back("Me smash you!");
    dialogue.push_back("*screech*");
}
std::vector<Item> Goblin::getLoot(const Player& player) const
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