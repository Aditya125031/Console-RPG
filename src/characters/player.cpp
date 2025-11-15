// Player.cpp
#include "../include/map.h"
#include "../include/tile.h"
#include"../include/player.h"
#include"../include/inventory.hpp"
#include"../include/items.h"
#include "../extern/pdcurses/curses.h"
#include"../include/game.h"

using namespace std;

Player::Player(Game& game_world, std::string name, PlayerType type)
    : Character(name, 100, 10), 
      world(game_world),  // <-- THIS LINE IS THE FIX
      type(type) 
{
    // The 'inventory' member is default-initialized automatically
    
    switch (type) {
    
    case PlayerType::Swordsman:
        this->health = 1500;
        this->attackPower = 150;
        this->mana = 200;
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
    this->max_mana = this->mana;
    this->baseMaxHealth = this->health;
    this->baseAttackPower = this->attackPower;
    this->baseMaxMana = this->mana;

    // 'printw' here is risky if ncurses isn't initialized yet,
    // but we'll leave it for now.
    printw("A new %s named %s has arrived!\n", get_type_string().c_str(), this->name.c_str());
}

// ... (reset_stats, get_type_string, show_details all look fine) ...
void Player::reset_stats() {
    this->attackPower = this->baseAttackPower;
    this->maxHealth = this->baseMaxHealth;
    this->max_mana = this->baseMaxMana;

    if (this->health > this->maxHealth) {
        this->health = this->maxHealth;
    }

    if (this->mana > this->max_mana) {
        this->mana = this->max_mana;
    }
}

std::string Player::get_type_string() const {
    switch (this->type) {
        case PlayerType::Swordsman:
            return "Swordsman";
        case PlayerType::Archer:
            return "Archer";
        case PlayerType::Mage:
            return "Mage";
        default:
            return "Unknown";
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
    std::shared_ptr<Weapon> current_weapon = this->inventory.equippedWeapon;
    
    if (current_weapon) 
    {
        
        if (current_weapon->special) {
            current_weapon->special_attack(*this, enemy, this->world);
        }
        else {
            this->world.add_log_message("Your " + current_weapon->get_item_name() + " does not have a special move.");
        }
    }
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
    this->health += amount;
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
std::string Player::move(int x, int y, Map& map) {
    // Implement movement logic here
    // Return a string describing the movement result
    return "Moved to position (" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

 int Player::getHPRegenVal(){
    return regen_hp_val;
 }

 void Player::setHPRegenVal(int a){
    regen_hp_val=a;
 }

 int Player::getManaRegenVal(){
    return regen_mana_val;
 }

 void Player::setManaRegenVal(int a){
    regen_mana_val=a;
 }

 int Player::getStepCount(){
    return step_count;
 }

 void Player::setStepCount(int a){
    step_count=a;
 }
