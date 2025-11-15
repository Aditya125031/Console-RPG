#pragma once
#include "enemy.h"

class OrcRaider : public Enemy {
public:
    OrcRaider();
    OrcRaider(Player& player);
    void triggerDialogue() const override;
};
