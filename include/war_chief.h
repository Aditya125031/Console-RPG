#pragma once
#include "enemy.h"

// 6. Goblin War Chief: Commands goblin forces; mini-boss with AoE buffs.
class GoblinWarChief : public Enemy {
private:
    bool isEnraged = false;
public:
    GoblinWarChief();
    void triggerDialogue() const override;
};