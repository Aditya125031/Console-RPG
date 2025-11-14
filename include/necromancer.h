#pragma once
#include "enemy.h"

class Necromancer : public Enemy {
private:
    int skeletonsSummoned = 0; 

public:
    Necromancer();
    Necromancer(Player& player);
    void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};
