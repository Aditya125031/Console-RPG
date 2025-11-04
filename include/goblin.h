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
    //void specialAbility(Character& target) override;
    void triggerDialogue() const override;
};

#endif //GOBLIN_H