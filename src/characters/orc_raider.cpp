#include "../include/orc_raider.h"
#include <iostream>
#include <cstdlib>

OrcRaider::OrcRaider()
    // Call base Enemy constructor: 
    // Enemy(name, health, attackPower, specialAttackPower)
    : Enemy("Orc Raider", 80, 12) // Normal ATK: 12, Special ATK: 20
{
    dialogue.push_back("For the Horde!");
    dialogue.push_back("Blood and thunder!");
    dialogue.push_back("I will crush you!");
}

void OrcRaider::triggerDialogue() const 
{
    if (!dialogue.empty()) {
        std::cout << get_name() << " bellows: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
