#include "../include/final_boss.h"
#include "../include/items.h"
#include <iostream>
#include <cstdlib>

LichLord::LichLord()
    : Enemy("Lich Lord", 300, 20) 
{
  
}
LichLord::LichLord(Player& player)
    : Enemy("Lich Lord", 300, 30) 
{
    std::string playerType = player.get_type_string();
    dialogue.push_back("You are a fool to challenge me.");
    dialogue.push_back("All your efforts are for nothing!");
    dialogue.push_back("I am eternal. You are... not.");
}
LichLord::~LichLord(){}

void LichLord::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << "'s voice echoes in your mind: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
void LichLord::specialAbility(Character& target) {
    std::cout << get_name() << " raises his staff and casts 'Chain of Shadow'!\n";
    int hits = (rand() % 5) + 2; 
    int damagePerHit = attackPower / 2;
    
    std::cout << "Bolts of dark energy leap towards you!\n";
    for (int i = 0; i < hits; ++i) {
        std::cout << "You are struck by a shadow bolt!\n";
        target.take_damage(damagePerHit);
    }
    std::cout << get_name() << " cackles at your pain.\n";
}
