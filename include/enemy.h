#pragma once
#include "character.h"
#include <string>
#include <vector>

class Enemy : public Character {
protected:
    std::vector<std::string> dialogue;
    int speed;
public:
    Enemy(std::string name, int health, int attackPower); 
    
    virtual ~Enemy() = default;
    std::string get_name() const; 
    virtual void specialAbility(Character& target) = 0;
    virtual void triggerDialogue() const = 0;
};
