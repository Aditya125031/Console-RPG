#pragma once
#include "enemy.h"

class InfernalImp : public Enemy {
public:
    InfernalImp();
    InfernalImp(Player& player);
    void triggerDialogue() const override;
};