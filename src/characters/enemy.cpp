#include "enemy.h"
enemy::enemy(std::string name, int health, int attackPower, int x, int y)
    : Character(name, health, attackPower), coord_x(x), coord_y(y) 
    {
}

std::string enemy::get_name() const {
    return this->name; 
}

int enemy::get_x() const {
    return this->coord_x; 
}

int enemy::get_y() const {
    return this->coord_y;
}