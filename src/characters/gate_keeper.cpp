#include "../include/gatekeeper.h"
#include "../include/items.h"
#include "../include/player.h"
#include <iostream>
#include <cstdlib>

Gate_Keeper::Gate_Keeper()
    : Enemy("Gate Keeper", 175, 25) 
{}
Gate_Keeper::Gate_Keeper(Player& player)
    : Enemy("Gate Keeper", 175, 25) 
{
    std::string playerType = player.get_type_string();
    dropLoot.push_back(make_shared<Angel_Blessings>());
    dropLoot.push_back(make_shared<Dragon_Breath>());
}