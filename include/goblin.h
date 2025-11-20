#ifndef GOBLIN_H
#define GOBLIN_H
#include "enemy.h" 
#include <chrono>
#include <string>
#include <vector>

class Goblin : public Enemy {
public:
    Goblin();
    Goblin(Player& player);
    virtual void triggerDialogue() const override;
    virtual void specialAbility(Character& target) override;
};
#endif