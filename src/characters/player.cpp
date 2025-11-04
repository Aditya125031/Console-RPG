// Player.cpp
#include "../include/player.h"
#include "../include/map.h"
#include "../include/tile.h"
#include "../include/colors.h"

#include "../extern/pdcurses/curses.h"

using namespace std;

Player::Player(std::string name, PlayerType type)
    : Character(name, 100, 10), type(type) {
    switch (type) {
        case PlayerType::Swordsman:
            this->health = 150;
            this->attackPower = 15;
            this->mana = 20;
            break;

        case PlayerType::Archer:
            this->health = 100;
            this->attackPower = 12;
            this->mana = 40;
            break;

        case PlayerType::Mage:
            this->health = 80;
            this->attackPower = 8;
            this->mana = 150;
            break;
    }
    this->maxHealth = this->health;

    printw("A new %s named %s has arrived!\n", get_type_string().c_str(), this->name.c_str());
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
    int row = 0;
    mvprintw(row++, 0, "---------------------");
    mvprintw(row++, 0, "Name: %s", this->name.c_str());
    mvprintw(row++, 0, "Type: %s", get_type_string().c_str());
    mvprintw(row++, 0, "Health: %d / %d", this->health, this->maxHealth); 
    mvprintw(row++, 0, "Attack Power: %d", this->attackPower);
    mvprintw(row++, 0, "Mana: %d", this->mana);
    mvprintw(row++, 0, "---------------------");
}

void Player::special_move(Character& enemy) 
{
    int row, col;
    getyx(stdscr, row, col); 
    
    mvprintw(row++, 0, "%s uses their special move: ", this->name.c_str());
    int damage = 0;
    std::string message = "";
    
    switch (this->type) {
        case PlayerType::Swordsman:
            if (this->mana >= 10) {
                message = "'Whirlwind Slash'!";
                damage = this->attackPower * 2.5; 
                this->mana -= 10;
            } else {
                message = "Not enough stamina!";
            }
            break;

        case PlayerType::Archer:
            if (this->mana >= 20) {
                message = "'Piercing Arrow'!";
                damage = this->attackPower * 3; 
                this->mana -= 20;
            } else {
                message = "Not enough stamina!";
            }
            break;

        case PlayerType::Mage:
            if (this->mana >= 30) {
                message = "'Fireball'!";
                damage =this->attackPower * 3.5;
                this->mana -= 30;
            } else {
                message = "Not enough mana!";
            }
            break;
    }
    mvprintw(row++, 0, "%s", message.c_str());
    
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

void Player::use_mana(int amount) {
    if (this->mana < amount) 
    {
        this->mana = 0;
    }
}

void Player::add_mana(int amount) {
    this->mana += amount;
    
    if (this->mana > this->max_mana) 
    {
        this->mana = this->max_mana;
    }
}

void Player::modify_max_mana(int amount) {
    this->max_mana += amount;
    if (this->mana > this->max_mana) {
        this->mana = this->max_mana;
    }
}

void Player::modify_attack(int amount) {
    this->attackPower += amount;
}

int Player::get_mana() const {
    return this->mana;
}

int Player::get_max_mana() const {
    return this->max_mana;
}

int Player::get_attack_power() const {
    return this->attackPower; 
}

void Player::add_health(int amount) {
    this->health += amount;\
    if (this->health > this->maxHealth) {
        this->health = this->maxHealth;
    }
}

void Player::modify_max_health(int amount) {
    this->maxHealth += amount;
    if (this->health > this->maxHealth) {
        this->health = this->maxHealth;
    }
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

void Player::update_mana_regen(std::chrono::steady_clock::time_point current_time) {
    (void)current_time; 
}
