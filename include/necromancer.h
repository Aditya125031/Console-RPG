#pragma once
#include "enemy.h"

class Necromancer : public Enemy {
private:
    int skeletonsSummoned = 0; // Track summons

public:
    Necromancer();

    void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};
