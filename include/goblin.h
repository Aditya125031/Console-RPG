#pragma once
#include "enemy.h"

class Goblin : public Enemy {
public:
    Goblin(); 

    void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};