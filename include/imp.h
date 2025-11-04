#pragma once
#include "enemy.h"

class Imp : public Enemy {
public:
    Imp(); // Constructor
    std::vector<Item> getLoot(const Player& player) const override;
    void triggerDialogue() const override;
};