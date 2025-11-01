#pragma once
#include "enemy.h"

class Imp : public Enemy {
public:
    Imp(); // Constructor

    // Override the pure virtual functions from Enemy
    void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};