#include <chrono>
#pragma once
#include "character.h"
#include "player.h"
#include "items.h"
#include <string>
#include <vector>

class Enemy : public Character {
private:        
    double normalAttackInterval;
protected:
    std::vector<std::string> dialogue;
    int speed;
    std::chrono::steady_clock::time_point normal_attack_ready;
   vector<shared_ptr<Item>>  dropLoot;
public:
    Enemy(std::string name, int health, int attackPower); 
    virtual ~Enemy() = default;
    std::string get_name() const; 
    std::chrono::steady_clock::time_point getNormalAttackReady() const;
    void setNormalAttackInterval(double sec);
    double getNormalAttackInterval() const;
    void setNormalAttackCooldown(float seconds);
    virtual void specialAbility(Character& target){};
    virtual void triggerDialogue() const {};
    vector<shared_ptr<Item>> getDropLoot() const { return dropLoot; }
};
