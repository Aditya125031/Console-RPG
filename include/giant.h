#pragma once
#include "enemy.h" 
#include <chrono>
#include <string>
#include <vector>

class GiantGoblin : public Enemy {
public:
    GiantGoblin();
    std::vector<Item> getLoot(const Player& player) const override;
    virtual void triggerDialogue() const override;
};
