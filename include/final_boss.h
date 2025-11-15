#pragma once
#include "enemy.h"

class LichLord : public Enemy {
public:
LichLord();
    LichLord(Player& player);
    virtual ~LichLord();
    void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};
