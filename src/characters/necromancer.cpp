#include "../include/necromancer.h"
#include <iostream>
#include <cstdlib>

Necromancer::Necromancer()
    // Moderate health, low direct attack
    : Enemy("Necromancer", 70, 6) 
{
    dialogue.push_back("The dead serve me!");
    dialogue.push_back("Rise, my minions!");
    dialogue.push_back("Your life force will sustain me.");
}

void Necromancer::triggerDialogue() const {
     if (!dialogue.empty()) {
        std::cout << get_name() << " intones: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}

void Necromancer::specialAbility(Character& target) {
    std::cout << get_name() << " chants dark words...\n";
    if (skeletonsSummoned < 3) { // Limit number of summons
        std::cout << "'Rise, Undead Servant!' A skeleton claws its way from the ground!\n";
        // In a more complex game, you would actually spawn a new weak enemy here.
        // For now, maybe the Necromancer gets a temporary attack boost?
        skeletonsSummoned++;
        attackPower += 2; // Gets stronger with more skeletons (temporary idea)
        std::cout << get_name() << " draws power from the summoned!\n";
    } else {
        std::cout << "But the air remains still. The necromancer drains some life instead!\n";
        // Life drain effect: deal small damage, heal self
        int drainDamage = 5;
        target.take_damage(drainDamage);
        this->health += drainDamage; // Heal self
        if (this->health > this->maxHealth) this->health = this->maxHealth; // Cap health
        std::cout << get_name() << " healed for " << drainDamage << " HP!\n";
    }
}