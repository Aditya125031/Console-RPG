#ifndef GOBLIN_H
#define GOBLIN_H
#include "enemy.h" 
#include <chrono>
#include <string>
#include <vector>

class Goblin : public Enemy {
public:
    Goblin(std::string name, int health, int attackPower);

    // We MUST provide implementations for the pure virtual functions
    virtual void specialAbility(Character& target) override;
    virtual void triggerDialogue() const override;
};
#endif