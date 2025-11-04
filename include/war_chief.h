#pragma once
#include "enemy.h"

class GoblinWarChief : public Enemy {
private:
    bool isEnraged = false;
public:
    GoblinWarChief();
    std::vector<Item> getLoot(const Player& player) const override;
    void triggerDialogue() const override;
};