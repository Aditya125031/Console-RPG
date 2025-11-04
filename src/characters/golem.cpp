#include "../include/golem.h"
#include <iostream>
#include <cstdlib>

BoneGolem::BoneGolem()
    // Tanky mini-boss
    : Enemy("Bone Golem", 200, 15) 
{
    // No real dialogue, just sounds
    dialogue.push_back("*CLACK... CLACK... RATTLE*");
    dialogue.push_back("*A hollow moan...*");
}

void BoneGolem::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " makes a sound: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}

void BoneGolem::specialAbility(Character& target) { 

    std::cout << get_name() << " uses 'Grave Harvest'!" << std::endl;
    std::cout << "It launches jagged bones at you and pulls in dark energy!\n";

    int damage_amount = 35;
    std::cout << get_name() << " strikes you for " << damage_amount << " damage!\n";
    target.take_damage(damage_amount);

    int heal_amount = 70;
    
    if (this->health < this->maxHealth) 
    {
        this->health += heal_amount;
        
        if (this->health > this->maxHealth) {
            this->health = this->maxHealth;
        }
        
        std::cout << get_name() << " knits its bones back together, healing to " << this->health << " HP!\n";
    } else {
        std::cout << get_name() << " is already at full health!\n";
    }
}