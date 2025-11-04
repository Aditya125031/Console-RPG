// Player.cpp
#include "../include/player.h"
#include "../include/map.h"
#include "../include/tile.h"

// ⭐️ Use the curses header instead of iostream
// #include <iostream> 
#include "../extern/pdcurses/curses.h" // Assuming this is your path

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

    // ⭐️ REPLACED std::cout with printw
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
    // We use mvprintw to display details neatly in the curses console
    // Assuming the calling function (Game::game_loop) has already called clear() and will call refresh()
    mvprintw(row++, 0, "---------------------");
    mvprintw(row++, 0, "Name: %s", this->name.c_str());
    mvprintw(row++, 0, "Type: %s", get_type_string().c_str());
    mvprintw(row++, 0, "Health: %d / %d", this->health, this->maxHealth); // Show max health too
    mvprintw(row++, 0, "Attack Power: %d", this->attackPower);
    mvprintw(row++, 0, "Mana: %d", this->mana);
    mvprintw(row++, 0, "Stamina: %d", this->stamina);
    mvprintw(row++, 0, "---------------------");
}

void Player::special_move(Character& enemy) 
{
    // Get the current cursor row to print messages clearly
    int row, col;
    getyx(stdscr, row, col); 
    
    // ⭐️ REPLACED std::cout with mvprintw
    mvprintw(row++, 0, "%s uses their special move: ", this->name.c_str());
    int damage = 0;
    std::string message = "";
    
    switch (this->type) {
        case PlayerType::Swordsman:
            if (this->stamina >= 25) {
                message = "'Whirlwind Slash'!";
                damage = this->attackPower * 2.5; // Example: 2.5x damage
                this->stamina -= 25;
            } else {
                message = "Not enough stamina!";
            }
            break;

        case PlayerType::Archer:
            if (this->stamina >= 20) {
                message = "'Piercing Arrow'!";
                damage = this->attackPower * 3; 
                this->stamina -= 20;
            } else {
                message = "Not enough stamina!";
            }
            break;

        case PlayerType::Mage:
            if (this->mana >= 30) {
                message = "'Fireball'!";
                damage = 40;
                this->mana -= 30;
            } else {
                message = "Not enough mana!";
            }
            break;
    }
    
    // Print the move message
    mvprintw(row++, 0, "%s", message.c_str());
    
    if (damage > 0) {
        enemy.take_damage(damage);
    }
    
    // The calling function (run_combat) will handle refresh/pausing
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

void Player::add_health(int amount) {
    this->health += amount;
    
    // Don't let health go over the max
    if (this->health > this->maxHealth) {
        this->health = this->maxHealth;
    }
}

void Player::modify_max_mana(int amount) {
    this->max_mana += amount;
    
    // Cap current mana if max is reduced
    if (this->mana > this->max_mana) {
        this->mana = this->max_mana;
    }
}
void Player::use_mana(int amount) {
    this->mana -= amount; 
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