#pragma once
#include "enemy.h"

class GoblinWarChief : public Enemy {
private:
    bool isEnraged = false;
public:
    GoblinWarChief();
    GoblinWarChief(Player& player);
    void triggerDialogue() const override;
};