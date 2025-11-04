#pragma once
#include "enemy.h"

class LichLord : public Enemy {
public:
    LichLord();
    virtual ~LichLord();
    std::vector<Item> getLoot(const Player& player) const override;
    void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};
