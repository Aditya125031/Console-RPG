#pragma once
#include "enemy.h"
class BoneGolem : public Enemy {
public:
    BoneGolem();
    BoneGolem(Player& player);
    void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};
