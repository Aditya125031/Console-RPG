#pragma once
#include "enemy.h"

// 5. Orc Raider: Savage warrior; stronger, smarter melee fighter.
class OrcRaider : public Enemy {
public:
    OrcRaider();
    void triggerDialogue() const override;
};
