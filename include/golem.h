#pragma once
#include "enemy.h"

// 8. Bone Golem: Undead construct; tanky mini-boss.
class BoneGolem : public Enemy {
public:
    BoneGolem();

    //void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};
