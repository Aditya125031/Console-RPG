#include "../include/golem.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

BoneGolem::BoneGolem()
    : Enemy("Bone Golem", 300, 30) 
{
    
    dialogue.push_back("*CLACK... CLACK... RATTLE*");
    dialogue.push_back("*A hollow moan...*");
}
BoneGolem::BoneGolem(Player& player)
    : Enemy("Bone Golem", 300, 30) 
{
     std::string playerType = player.get_type_string();

    if (playerType == "Swordsman") 
    {
        dropLoot.push_back(make_shared<Dragon_Armor>());
        dropLoot.push_back(make_shared<Elixir_Of_Life>());
    } else if (playerType == "Archer") 
    {
        dropLoot.push_back(make_shared<Divine_Aegis>());
        dropLoot.push_back(make_shared<Angel_Blessings>());
        dropLoot.push_back(make_shared<Dragon_Breath>());
    } else if (playerType == "Mage") 
    {
        dropLoot.push_back(make_shared<Aether_Robe>()); 
        dropLoot.push_back(make_shared<Aether_Elixir>());
    }
    dialogue.push_back("*CLACK... CLACK... RATTLE*");
    dialogue.push_back("*A hollow moan...*");
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

    int heal_amount = 30;
    
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