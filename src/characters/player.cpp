// Player.cpp
#include "player.h"
#include <iostream> 
Player::Player(std::string name, PlayerType type)
    : Character(name, 100, 10), type(type) {
    switch (type) {
        case PlayerType::Swordsman:
            this->health = 150;
            this->attackPower = 15;
            this->mana = 20;
            this->stamina = 100;
            break;

        case PlayerType::Archer:
            this->health = 100;
            this->attackPower = 12;
            this->mana = 40;
            this->stamina = 120;
            break;

        case PlayerType::Mage:
            this->health = 80;
            this->attackPower = 8;
            this->mana = 150;
            this->stamina = 50;
            break;
    }
    this->maxHealth = this->health;

    std::cout << "A new " << get_type_string() << " named " << this->name << " has arrived!" << std::endl;
}

std::string Player::get_type_string() const {
    switch (this->type) {
        case PlayerType::Swordsman: return "Swordsman";
        case PlayerType::Archer:    return "Archer";
        case PlayerType::Mage:      return "Mage";
        default:                    return "Unknown";
    }
}

void Player::show_details() const {
    std::cout << "---------------------\n"
              << "Name: " << this->name << "\n"
              << "Type: " << get_type_string() << "\n"
              << "Health: " << this->health << " / " << this->maxHealth << "\n" // Show max health too
              << "Attack Power: " << this->attackPower << "\n"
              << "Mana: " << this->mana << "\n"
              << "Stamina: " << this->stamina << "\n"
              << "---------------------\n";
}

void Player::special_move() {
    std::cout << this->name << " uses their special move: ";
    switch (this->type) {
        case PlayerType::Swordsman:
            std::cout << "'Whirlwind Slash'!" << std::endl;
            this->stamina -= 25;
            break;
        case PlayerType::Archer:
            std::cout << "'Piercing Arrow'!" << std::endl;
            this->stamina -= 20;
            break;
        case PlayerType::Mage:
            std::cout << "'Fireball'!" << std::endl;
            this->mana -= 30;
            break;
    }
}