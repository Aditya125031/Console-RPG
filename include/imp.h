#pragma once
#include "enemy.h"

class Imp : public Enemy {
public:
    Imp(); 
    Imp(Player& player);
    void triggerDialogue() const override;
};