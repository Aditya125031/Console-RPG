// Character.h
#pragma once 
#include <string>

class Character 
{
protected:
    std::string name;
    int health;
    int maxHealth;
    int attackPower;

public:
    Character(std::string name, int health, int attackPower);
    
    virtual ~Character() = default;

    void take_damage(int damage);
    bool isAlive() const; 
    
    virtual void attack(Character& target);

    std::string get_name() const;
    int get_health() const;
    int get_max_health() const;
};