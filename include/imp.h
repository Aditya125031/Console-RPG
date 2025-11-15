#pragma once
#include "enemy.h"

class Imp : public Enemy {
public:
    Imp(); // Constructor
    Imp(Player& player);
    void triggerDialogue() const override;
};