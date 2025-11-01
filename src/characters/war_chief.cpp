#include "war_chief.h"
#include <iostream>
#include <cstdlib>

GoblinWarChief::GoblinWarChief()
    // Mini-boss stats
    : Enemy("Goblin War Chief", 150, 14) 
{
    dialogue.push_back("For the Boss! Attack!");
    dialogue.push_back("You'll make a fine trophy!");
    dialogue.push_back("More guts! More glory!");
}

void GoblinWarChief::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " barks: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
