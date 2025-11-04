// Character.h
#pragma once 
#include <string>
using namespace std;

class Character 
{
protected:
    std::string name;
    int health;
    int maxHealth;
    int attackPower;
    int coord_x, coord_y;

public:
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
    int getAttackPower() const;
    virtual int get_x(){return 0;}
    virtual int get_y(){return 0;}
    virtual void set_x(int){};
    virtual void set_y(int){};
};