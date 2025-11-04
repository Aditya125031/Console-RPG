#ifndef GOBLIN_H
#define GOBLIN_H
#include "enemy.h" 
#include <chrono>
#include <string>
#include <vector>

class Goblin : public Enemy {
public:
    Goblin();
    // Goblin(std::string name, int health, int attackPower);

    // We MUST provide implementations for the pure virtual functions
    //void specialAbility(Character& target) override;
    std::vector<Item> getLoot(const Player& player) const override;
    virtual void triggerDialogue() const override;
    virtual void specialAbility(Character& target) override;
};

#endif //GOBLIN_H