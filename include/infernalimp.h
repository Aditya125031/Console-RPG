#pragma once
#include "enemy.h"

class InfernalImp : public Enemy {
public:
    InfernalImp();
    std::vector<Item> getLoot(const Player& player) const override;
    void triggerDialogue() const override;
};