// Player.cpp
#include "../include/player.h"
#include "../include/map.h"
#include "../include/tile.h"
#include "../include/colors.h"
#include <iostream> 
using namespace std;


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

// Note: 'Character' with a capital 'C'
void Player::special_move(Character& enemy) 
{
    std::cout << this->name << " uses their special move: ";
    int damage = 0;
    switch (this->type) {
        case PlayerType::Swordsman:
            if (this->stamina >= 25) {
                std::cout << "'Whirlwind Slash'!" << std::endl;
                damage = this->attackPower * 2.5; // Example: 2.5x damage
                this->stamina -= 25;
            } else {
                std::cout << "Not enough stamina!" << std::endl;
            }
            break;

        case PlayerType::Archer:
            if (this->stamina >= 20) {
                std::cout << "'Piercing Arrow'!" << std::endl;
                damage = this->attackPower * 3; 
                this->stamina -= 20;
            } else {
                std::cout << "Not enough stamina!" << std::endl;
            }
            break;

        case PlayerType::Mage:
            if (this->mana >= 30) {
                std::cout << "'Fireball'!" << std::endl;
                damage = 40;
                this->mana -= 30;
            } else {
                std::cout << "Not enough mana!" << std::endl;
            }
            break;
    }
    if (damage > 0) {
        enemy.take_damage(damage);
    }
}
int Player::get_x() {
    return this->coord_x;
}

int Player::get_y() {
    return this->coord_y;
}

void Player::set_x(int a) {
    this->coord_x=a;
}

void Player::set_y(int a){
    this->coord_y=a;
}

std::chrono::steady_clock::time_point Player::get_normal_attack_ready() const {
    return this->normal_attack_ready;
}

std::chrono::steady_clock::time_point Player::get_special_attack_ready() const {
    return this->special_attack_ready;
}

void Player::set_normal_attack_cooldown(float seconds) {
    this->normal_attack_ready = std::chrono::steady_clock::now() + 
        std::chrono::microseconds(static_cast<int>(seconds * 1000000));
}

void Player::set_special_attack_cooldown(float seconds) {
    this->special_attack_ready = std::chrono::steady_clock::now() + 
        std::chrono::microseconds(static_cast<int>(seconds * 2500000));
}
void Player::modify_max_health(int amount) {
    this->maxHealth += amount;

    // If a debuff lowers max health, cap the current health
    if (this->health > this->maxHealth) {
        this->health = this->maxHealth;
    }
}

// This is for potions, to add to CURRENT health
void Player::add_health(int amount) {
    this->health += amount;
    
    // Don't let health go over the max
    if (this->health > this->maxHealth) {
        this->health = this->maxHealth;
    }
}

// I recommend renaming 'modify_mana' to 'modify_max_mana' for clarity
// This is for equippable items
void Player::modify_max_mana(int amount) {
    this->max_mana += amount;
    
    // Cap current mana if max is reduced
    if (this->mana > this->max_mana) {
        this->mana = this->max_mana;
    }
}
void Player::use_mana(int amount) {
    this->mana -= amount; // <-- Corrected: use 'mana'
    if (this->mana < 0) {
        this->mana = 0;
    }
}
void Player::setSpecialAttackInterval(float seconds) {
    specialAttackInverval = seconds;
    return;
}
double Player::getSpecialAttackInterval() {
    return specialAttackInverval;
}
int Player::get_mana() const{
    return mana;
}
void Player::setNormalAttackInterval(float seconds) {
    attackInterval = seconds;
    return;
}