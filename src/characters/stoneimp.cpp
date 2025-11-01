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

void StoneImp::specialAbility(Character& target) {
    if (!isHardened) {
        std::cout << get_name() << " uses 'Harden Skin'!\n";
        std::cout << "Its rocky hide becomes incredibly tough!\n";
        // Simulate defense boost (e.g., reduce its attack temporarily)
        // A real implementation would modify take_damage or add a defense stat
        attackPower = 1; // Greatly reduce attack while hardened
        isHardened = true;
    } else {
        std::cout << get_name() << " tries to harden further, but fails!\n";
        // Maybe do a small attack if it fails?
        attack(target);
    }
    // Note: We need a way for 'isHardened' to wear off eventually!
    // This could be done by adding a turn counter or checking time in run_combat.
}

/* // Optional: Override take_damage to implement defense
void StoneImp::take_damage(int damage) {
    if (isHardened) {
        std::cout << getName() << "'s hardened skin absorbs most of the blow!\n";
        Character::take_damage(damage / 3); // Take only 1/3 damage
    } else {
        Character::take_damage(damage); // Take normal damage
    }
}
*/