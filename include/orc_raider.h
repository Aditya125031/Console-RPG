#pragma once
#include "enemy.h"

class OrcRaider : public Enemy {
public:
    OrcRaider();
    std::vector<Item> getLoot(const Player& player) const override;
    void triggerDialogue() const override;
};
