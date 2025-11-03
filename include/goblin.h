#ifndef GOBLIN_H
#define GOBLIN_H
#include "enemy.h" 
#include <chrono>
#include <string>
#include <vector>

class Goblin : public Enemy {
public:
    Goblin(std::string name, int health, int attackPower);

    virtual void triggerDialogue() const override;
};
#endif