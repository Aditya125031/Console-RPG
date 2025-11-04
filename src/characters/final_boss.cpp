#include "../include/final_boss.h"
#include "../include/items.h"
#include <iostream>
#include <cstdlib>

LichLord::LichLord()
    // Final Boss stats
    : Enemy("Lich Lord", 300, 20) 
{
    dialogue.push_back("You are a fool to challenge me.");
    dialogue.push_back("All your efforts are for nothing!");
    dialogue.push_back("I am eternal. You are... not.");
}
LichLord::~LichLord()
{
    // This empty body is all the linker needs
}
void LichLord::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << "'s voice echoes in your mind: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
std::vector<Item> LichLord::getLoot(const Player& player) const
{
    std::vector<Item> allLoot = Enemy::getLoot(player);

    std::string playerType = player.get_type_string();

    if (playerType == "Swordsman") 
    {
        allLoot.push_back(Shinketsu_Sword()); 
    } else if (playerType == "Archer") 
    {
        allLoot.push_back(Void_Embrace()); 
    } else if (playerType == "Mage") 
    {
        allLoot.push_back(Elder_Wand()); 
    }
    return allLoot;
}
void LichLord::specialAbility(Character& target) {
    std::cout << get_name() << " raises his staff and casts 'Chain of Shadow'!\n";
    // A multi-hit attack
    int hits = (rand() % 3) + 2; // Hits 2 to 4 times
    int damagePerHit = attackPower / 2; // 10 damage per hit
    
    std::cout << "Bolts of dark energy leap towards you!\n";
    for (int i = 0; i < hits; ++i) {
        std::cout << "You are struck by a shadow bolt!\n";
        target.take_damage(damagePerHit);
    }
    std::cout << get_name() << " cackles at your pain.\n";
}
