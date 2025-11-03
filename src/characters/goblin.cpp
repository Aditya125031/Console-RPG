#include "../include/goblin.h"
#include <iostream> 
#include <cstdlib> // For rand()
#include <vector>  // For std::vector
#include <string>  // For std::string

Goblin::Goblin(std::string name, int health, int attackPower)
    : Enemy(name, health, attackPower) // Pass the arguments to the parent Enemy class
{
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