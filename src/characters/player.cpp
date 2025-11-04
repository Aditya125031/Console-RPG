// Player.cpp
#include "../include/player.h"
#include "../include/map.h"
#include "../include/tile.h"
#include "../extern/pdcurses/curses.h"

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
    mvprintw(row++, 0, "Health: %d / %d", this->health, this->maxHealth);
    mvprintw(row++, 0, "Attack Power: %d", this->attackPower);
    mvprintw(row++, 0, "Mana: %d", this->mana);
    mvprintw(row++, 0, "Stamina: %d", this->stamina);
    mvprintw(row++, 0, "---------------------");
}

void Player::special_move(Character& enemy) {
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
    this->coord_x = a;
}

void Player::set_y(int a) {
    this->coord_y = a;
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

void Player::setNormalAttackInterval(double sec) {
    this->normalAttackInterval = sec;
}

double Player::getNormalAttackInterval() const {
    return this->normalAttackInterval;
}

void Player::setSpecialAttackInterval(double sec) {
    this->specialAttackInterval = sec;
}

double Player::getSpecialAttackInterval() const {
    return this->specialAttackInterval;
}

// Missing method implementations that were declared in the header
void Player::add_mana(int amount) {
    this->mana += amount;
    if (this->mana > this->max_mana) {
        this->mana = this->max_mana;
    }
}

void Player::modify_maxmana(int amount) {
    this->max_mana += amount;
    if (this->mana > this->max_mana) {
        this->mana = this->max_mana;
    }
}

void Player::update_mana_regen(std::chrono::steady_clock::time_point current_time) {
    // Implement mana regeneration logic here
    // This would typically be called periodically in the game loop
}

int Player::get_mana() const {
    return this->mana;
}

int Player::get_max_mana() const {
    return this->max_mana;
}

std::string Player::move(int x, int y, Map& map) {
    // Implement movement logic here
    // Return a string describing the movement result
    return "Moved to position (" + std::to_string(x) + ", " + std::to_string(y) + ")";
}