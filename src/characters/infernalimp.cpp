#include "../include/infernalimp.h"
#include <iostream>
#include <cstdlib>

InfernalImp::InfernalImp()
    // Glass cannon: High attack, low-ish health
    : Enemy("Infernal Imp", 60, 18) 
{
    dialogue.push_back("Burn, fleshling, burn!");
    dialogue.push_back("The Black Pit awaits!");
    dialogue.push_back("*giggles manically*");
}

void InfernalImp::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " hisses: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
