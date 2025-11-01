#include "stoneimp.h"
#include <iostream>
#include <cstdlib>

StoneImp::StoneImp()
    // High health, low attack
    : Enemy("Stone Imp", 80, 5) 
{
    originalAttackPower = attackPower; // Store initial attack power
    dialogue.push_back("*rumble*...");
    dialogue.push_back("Flesh is weak. Stone is eternal.");
    dialogue.push_back("You cannot break me.");
}

void StoneImp::triggerDialogue() const {
     if (!dialogue.empty()) {
        std::cout << get_name() << " grinds out: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
