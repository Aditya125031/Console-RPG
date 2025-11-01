#pragma once
#include "enemy.h"

class Imp : public Enemy {
public:
    Imp(); // Constructor
    void triggerDialogue() const override;
};