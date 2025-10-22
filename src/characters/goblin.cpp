#include "goblin.h"
#include <iostream> 
#include <cstdlib> // For rand()
#include <vector>  // For std::vector
#include <string>  // For std::string

// CORRECTED: This constructor now matches the one in goblin.h
Goblin::Goblin(std::string name, int health, int attackPower)
    : Enemy(name, health, attackPower) // Pass the arguments to the parent Enemy class
{
    // 'dialogue' is an inherited member, so we can just use it
    dialogue.push_back("Shiny things for Grug!");
    dialogue.push_back("Me smash you!");
    dialogue.push_back("*screech*");
}

// This function implementation is great
void Goblin::triggerDialogue() const 
{
    if (!dialogue.empty()) {
        std::cout << get_name() << " screeches: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}

// This function implementation is also great
void Goblin::specialAbility(Character& target) 
{
    std::cout << get_name() << " uses 'Reckless Stab'!\n";
    int damage;
    if (rand() % 2 == 0) {
        damage = attackPower * 2; // Double damage!
        std::cout << "A critical hit!\n";
    } else {
        damage = 1; // Missed!
        std::cout << "It barely grazes you!\n";
    }
    
    // 'target' is the player. Make the player take the damage.
    target.take_damage(damage);
}
