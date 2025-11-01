#include "imp.h"
#include <iostream>
#include <cstdlib> // For rand()

Imp::Imp()
    // Call base Enemy constructor: Enemy(name, health, attackPower)
    : Enemy("Imp", 35, 10) 
{
    // Add unique dialogue lines
    dialogue.push_back("*cackles*");
    dialogue.push_back("Can't catch me!");
    dialogue.push_back("Hehehe!");
}

void Imp::triggerDialogue() const 
{
    if (!dialogue.empty()) 
    {
        std::cout << get_name() << " shrieks: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}

void Imp::specialAbility(Character& target) {
    std::cout << get_name() << " uses 'Vicious Bite'!\n";
    // A quick attack that might cause bleeding (though we won't implement status effects yet)
    int damage = attackPower + (rand() % 4); // Base damage + 0 to 3 bonus
    std::cout << "It's surprisingly painful!\n";
    target.take_damage(damage);
}
