#include <chrono>
#pragma once
#include "character.h"
#include <string>
#include <vector>

class Enemy : public Character {
protected:
    std::vector<std::string> dialogue;
    std::chrono::steady_clock::time_point normal_attack_ready;
    int specialAttackPower;
public:
    Enemy(std::string name, int health, int attackPower, int specialAttackPower); 

    virtual ~Enemy() = default;
    std::string get_name() const; 
    std::chrono::steady_clock::time_point getNormalAttackReady() const;
    void setNormalAttackCooldown(float seconds);
    virtual void specialAbility(Character& target) = 0;
    virtual void triggerDialogue() const = 0;
};
