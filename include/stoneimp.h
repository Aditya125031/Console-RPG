#pragma once
#include "enemy.h"

class StoneImp : public Enemy {
private:
    bool isHardened = false; // Internal state for its ability
    int originalAttackPower; // To restore after hardening wears off (if needed)

public:
    StoneImp(); // Constructor
    StoneImp(Player& player);
    void triggerDialogue() const override;
    // Optional: Override take_damage to implement defense boost
    // void take_damage(int damage) override;
};