#pragma once
#include "enemy.h"
class BoneGolem : public Enemy {
public:
    BoneGolem();

    void specialAbility(Character& target) override;
    std::vector<Item> getLoot(const Player& player) const override;
    void triggerDialogue() const override;
};
