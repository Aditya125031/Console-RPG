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
