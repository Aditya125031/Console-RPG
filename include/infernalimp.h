#pragma once
#include "enemy.h"

// 7. Infernal Imp: Advanced demon; strong fire magic and death explosion.
class InfernalImp : public Enemy {
public:
    InfernalImp();
    void triggerDialogue() const override;

    // We can't do a "death explosion" in the destructor,
    // so its special ability will be a 'reckless' attack
    // that also damages itself.
};