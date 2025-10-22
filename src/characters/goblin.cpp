
#include "goblin.h"
#include "enemy.h"
#include <iostream> 
#include <cstdlib>  

Goblin::Goblin()
    : Enemy("Goblin", 50, 8) 
{
    dialogue.push_back("Shiny things for Grug!");
    dialogue.push_back("Me smash you!");
    dialogue.push_back("*screech*");
}


void Goblin::triggerDialogue() const 
{
    std::cout << get_name() << " screeches: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
}

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