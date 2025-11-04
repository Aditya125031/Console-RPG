#include <chrono>
#pragma once
#include "character.h"
#include <string>
#include <vector>
#include "item.h"

class Enemy : public Character {
private:        
    double normalAttackInterval;
protected:
    std::vector<std::string> dialogue;
    int speed;
public:
    Enemy(std::string name, int health, int attackPower); 

    virtual ~Enemy() = default;
    std::string get_name() const; 
    std::chrono::steady_clock::time_point getNormalAttackReady() const;
    void setNormalAttackInterval(double sec);
    double getNormalAttackInterval() const;
    virtual void specialAbility (Character& target) {};
    virtual void triggerDialogue() const {};
    void Enemy::setNormalAttackCooldown(float seconds);
    std::vector<Item> getLoot(const Player& player) const;
};
