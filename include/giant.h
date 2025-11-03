#pragma once
#include "enemy.h"

class GiantGoblin : public Enemy {
public:
    GiantGoblin();

    void triggerDialogue() const override;
};
