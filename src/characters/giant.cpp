#include "../include/giant.h"
#include <iostream>
#include <cstdlib>

GiantGoblin::GiantGoblin()
    // High health, high attack
    : Enemy("Giant Goblin", 120, 15) 
{
    dialogue.push_back("Me bigger! Me stronger!");
    dialogue.push_back("CRUSH PUNY THING!");
    dialogue.push_back("WAAAGH!");
}

void GiantGoblin::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " roars: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
