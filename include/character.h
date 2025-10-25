// Character.h
#pragma once 
#include <chrono>
#include <string>
using namespace std;

class Character 
{
protected:
    std::string name;
    int health;
    int maxHealth;
    int attackPower;

public:
    int x, y;
    Character(std::string name, int health, int attackPower);
    
    virtual ~Character() = default;

    void take_damage(int damage);
    bool isAlive() const; 
    void move(int dx, int dy);
    virtual void attack(Character& target);
    void takeDamage(int damage);
    std::string get_name() const;
    int get_health() const;
    int get_max_health() const;
};