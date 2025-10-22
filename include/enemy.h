#pragma once
#include "character.h"
#include <string>
#include <vector>

class Enemy : public Character {
protected:
    std::vector<std::string> dialogue;

public:
    Enemy(std::string name, int health, int attackPower)
        : Character(name, health, attackPower) {} 

    virtual ~Enemy() = default;
    virtual void specialAbility(Character& target) = 0;
    virtual void triggerDialogue() const = 0;
};