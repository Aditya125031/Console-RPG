#pragma once
#include "enemy.h"

class Necromancer : public Enemy {
private:
    int skeletonsSummoned = 0; 

public:
    Necromancer();
    std::vector<Item> getLoot(const Player& player) const override;
    void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};
