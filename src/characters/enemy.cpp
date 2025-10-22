#include "enemy.h"
Enemy::Enemy(std::string name, int health, int attackPower)
    : Character(name, health, attackPower)
    {
}

std::string Enemy::get_name() const {
    return this->name; 
}