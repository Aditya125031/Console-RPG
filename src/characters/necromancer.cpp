#include "../include/necromancer.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

Necromancer::Necromancer()
    : Enemy("Necromancer", 180, 24) 
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
Necromancer::Necromancer(Player& player)
    : Enemy("Necromancer", 140, 18) 
{
     std::string playerType = player.get_type_string();

    if (playerType == "Swordsman") 
    {
        dropLoot.push_back(make_shared<God_Slayer>());
        dropLoot.push_back(make_shared<Aether_Elixir>());
        dropLoot.push_back(make_shared<Elixir_Of_Life>());
    } else if (playerType == "Archer") 
    {
        dropLoot.push_back(make_shared<Void_Embrace>());
        dropLoot.push_back(make_shared<Elixir_Of_Life>());
        dropLoot.push_back(make_shared<Aether_Elixir>());
    } else if (playerType == "Mage") 
    {
        dropLoot.push_back(make_shared<Oblivion_Shard>()); 
        dropLoot.push_back(make_shared<Aether_Elixir>());
        dropLoot.push_back(make_shared<Elixir_Of_Life>());
    }
    dialogue.push_back("The dead serve me!");
    dialogue.push_back("Rise, my minions!");
    dialogue.push_back("Your life force will sustain me.");
}

void Necromancer::specialAbility(Character& target) {
    std::cout << get_name() << " chants dark words...\n";
    if (skeletonsSummoned < 3) 
    {
        std::cout << "'Rise, Undead Servant!' A skeleton claws its way from the ground!\n";
        skeletonsSummoned++;
        attackPower += 2; 
        std::cout << get_name() << " draws power from the summoned!\n";
    } else {
        std::cout << "But the air remains still. The necromancer drains some life instead!\n";
        int drainDamage = 20;
        target.take_damage(drainDamage);
        this->health += drainDamage; 
        if (this->health > this->maxHealth) this->health = this->maxHealth; 
        std::cout << get_name() << " healed for " << drainDamage << " HP!\n";
    }
}