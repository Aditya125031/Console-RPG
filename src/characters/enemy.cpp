#include "../include/enemy.h"
enemy::enemy(std::string name, int health, int attackPower, int x, int y)
    : Character(name, health, attackPower), coord_x(x), coord_y(y) 
    {
}

std::string Enemy::get_name() const {
    return this->name; 
}