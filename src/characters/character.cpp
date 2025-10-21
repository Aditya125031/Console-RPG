
#include "Character.h" 
#include <iostream>

Character::Character(std::string name, int health, int attackPower) 
    : name(name), health(health), maxHealth(health), attackPower(attackPower) {
}

bool Character::isAlive() const {
    return this->health > 0;
}

void Character::attack(Character& target) {
    std::cout << this->name << " attacks " << target.get_name() << "!\n";
    target.take_damage(this->attackPower);
}

void Character::take_damage(int damage) {
    this->health -= damage;
    std::cout << this->name << " takes " << damage << " damage!" << std::endl;
    
    if (this->health < 0) {
        this->health = 0;
    }
    
    if (!isAlive()) {
        std::cout << this->name << " has been defeated!" << std::endl;
    }
}
std::string Character::get_name() const {
    return this->name;
}

int Character::get_health() const {
    return this->health;
}

int Character::get_max_health() const {
    return this->maxHealth;
}