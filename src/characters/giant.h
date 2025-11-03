#pragma once
#include "enemy.h"

class GiantGoblin : public Enemy {
public:
    GiantGoblin();

    void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};
