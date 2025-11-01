#include "giantgoblin.h"
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

void GiantGoblin::specialAbility(Character& target) {
    std::cout << get_name() << " uses 'Mighty Smash'!\n";
    // High damage attack
    int damage = attackPower * 2;
    std::cout << "It brings its club down hard!\n";
    target.take_damage(damage);
}
