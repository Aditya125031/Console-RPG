#include <chrono>
#pragma once
#include "character.h"
#include <string>
#include <vector>

class Enemy : public Character {
private:
    double specialAttackInterval;
protected:
    std::vector<std::string> dialogue;
    int speed;
    std::chrono::steady_clock::time_point normal_attack_ready;
public:
    Enemy(std::string name, int health, int attackPower); 

    virtual ~Enemy() = default;
    std::string get_name() const; 
    std::chrono::steady_clock::time_point getNormalAttackReady() const;
    void setNormalAttackCooldown(float seconds);
    virtual void specialAbility(Character& target){};
    virtual void triggerDialogue() const {};
};
