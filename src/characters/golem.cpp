#include "../include/golem.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

BoneGolem::BoneGolem()
    : Enemy("Bone Golem", 280, 20) 
{
    
    dialogue.push_back("*CLACK... CLACK... RATTLE*");
    dialogue.push_back("*A hollow moan...*");
}
std::vector<Item> BoneGolem::getLoot(const Player& player) const
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
void BoneGolem::triggerDialogue() const {
    if (!dialogue.empty()) {
        std::cout << get_name() << " makes a sound: \"" << dialogue[rand() % dialogue.size()] << "\"\n";
    }
}
void BoneGolem::specialAbility(Character& target) { 

    std::cout << get_name() << " uses 'Grave Harvest'!" << std::endl;
    std::cout << "It launches jagged bones at you and pulls in dark energy!\n";

    int damage_amount = 35;
    std::cout << get_name() << " strikes you for " << damage_amount << " damage!\n";
    target.take_damage(damage_amount);

    int heal_amount = 70;
    
    if (this->health < this->maxHealth) 
    {
        this->health += heal_amount;
        
        if (this->health > this->maxHealth) {
            this->health = this->maxHealth;
        }
        
        std::cout << get_name() << " knits its bones back together, healing to " << this->health << " HP!\n";
    } else {
        std::cout << get_name() << " is already at full health!\n";
    }
}