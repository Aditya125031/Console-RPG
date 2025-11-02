#pragma once
#include "enemy.h"

// 10. Lich Lord: Final boss; controls dark magic.
class LichLord : public Enemy {
public:
    LichLord();

    void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};
