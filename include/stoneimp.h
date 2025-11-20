#pragma once
#include "enemy.h"

class StoneImp : public Enemy {
private:
    bool isHardened = false;
    int originalAttackPower;

public:
    StoneImp(); 
    StoneImp(Player& player);
    void triggerDialogue() const override;
};