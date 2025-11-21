#pragma once
#include "enemy.h"

class Gate_Keeper : public Enemy {
public:
    Gate_Keeper();
    Gate_Keeper(Player& player);
};
