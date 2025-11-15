#pragma once
#include "enemy.h" 
#include <chrono>
#include <string>
#include <vector>

class GiantGoblin : public Enemy {
public:
    GiantGoblin();
    GiantGoblin(Player& player);
    virtual void triggerDialogue() const override;
};
