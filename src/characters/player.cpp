// Player.cpp
#include "../include/map.h"
#include "../include/tile.h"
#include "../include/player.h"
#include "../include/inventory.hpp"
#include "../include/items.h"
#include "../extern/pdcurses/curses.h"
#include "../include/game.h"

using namespace std;

Player::Player(Game& game_world, std::string name, PlayerType type)
    : Character(name, 100, 10), 
      world(game_world),  
      type(type) 
{
    switch (type) {
    
    case PlayerType::Swordsman:
        this->health = 120;
        this->attackPower = 15;
        this->mana = 20;
        this->regen_hp_time = 5;
        this->regen_mana_time = 10;
        this->inventory.equippedWeapon = make_shared<Iron_Sword>();
        break;

    case PlayerType::Archer:
        this->health = 100;
        this->attackPower = 13;
        this->mana = 60;
        this->regen_hp_time = 8;
        this->regen_mana_time = 8;
        this->inventory.equippedWeapon = make_shared<Wooden_Bow>();
        break;

    case PlayerType::Mage:
        this->health = 80;
        this->attackPower = 11;
        this->mana = 100;
        this->regen_hp_time = 10;
        this->regen_mana_time = 5;
        this->inventory.equippedWeapon = make_shared<Novice_Wand>();
        break;
    }
    this->maxHealth = this->health;
    this->max_mana = this->mana;
    this->baseMaxHealth = this->health;
    this->baseAttackPower = this->attackPower;
    this->baseMaxMana = this->mana;
    printw("A new %s named %s has arrived!\n", get_type_string().c_str(), this->name.c_str());
    this_thread::sleep_for(chrono::seconds(1));
    flushinp();
}

void Player::reset_stats() {
    this->attackPower = this->baseAttackPower;
    this->maxHealth = this->baseMaxHealth;
    this->max_mana = this->baseMaxMana;

    if (this->health > this->maxHealth)
    {
        this->health = this->maxHealth;
    }

    if (this->mana > this->max_mana)
    {
        this->mana = this->max_mana;
    }
}

std::string Player::get_type_string() const
{
    switch (this->type)
    {
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

#include <string> // Ensure this is included at the top of your player.cpp

/**
 * @brief Displays the player's stats in a decorative window.
 */
void Player::show_details() const
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols); // Get screen dimensions

    // --- Define Box Dimensions ---
    int boxW = 50;
    int boxH = 14;
    int startY = (rows - boxH) / 2;
    int startX = (cols - boxW) / 2;

    // --- Draw the Box Border (White) ---
    attron(COLOR_PAIR(6)); 
    mvaddch(startY, startX, ACS_ULCORNER);
    mvaddch(startY, startX + boxW, ACS_URCORNER);
    mvaddch(startY + boxH, startX, ACS_LLCORNER);
    mvaddch(startY + boxH, startX + boxW, ACS_LRCORNER);
    mvhline(startY, startX + 1, ACS_HLINE, boxW - 1);
    mvhline(startY + boxH, startX + 1, ACS_HLINE, boxW - 1);
    mvvline(startY + 1, startX, ACS_VLINE, boxH - 1);
    mvvline(startY + 1, startX + boxW, ACS_VLINE, boxH - 1);
    attroff(COLOR_PAIR(6));

    // --- Title (Cyan) ---
    attron(COLOR_PAIR(1) | A_BOLD); 
    mvprintw(startY + 1, startX + (boxW - 16) / 2, "--- PLAYER STATS ---");
    attroff(COLOR_PAIR(1) | A_BOLD);

    // --- Stats ---
    int row = startY + 3;
    int statX = startX + 4;
    
    // Name and Class (White)
    attron(COLOR_PAIR(6));
    mvprintw(row,     statX, "Name:  %s", this->name.c_str());
    mvprintw(row + 1, statX, "Class: %s", get_type_string().c_str());
    attroff(COLOR_PAIR(6));

    // Health (Green)
    mvprintw(row + 2, statX, "Health: ");
    attron(COLOR_PAIR(2)); // Green
    printw("%d / %d", this->health, this->maxHealth);
    attroff(COLOR_PAIR(2));

    // Mana (Blue)
    mvprintw(row + 3, statX, "Mana:   ");
    attron(COLOR_PAIR(3)); // Blue
    printw("%d / %d", this->mana, this->max_mana);
    attroff(COLOR_PAIR(3));

    // Attack (Red)
    mvprintw(row + 4, statX, "Attack: ");
    attron(COLOR_PAIR(4)); // Red
    printw("%d", this->attackPower);
    attroff(COLOR_PAIR(4));

    // --- Separator ---
    attron(COLOR_PAIR(6));
    mvaddch(startY + 9, startX, ACS_LTEE);
    mvhline(startY + 9, startX + 1, ACS_HLINE, boxW - 2);
    mvaddch(startY + 9, startX + boxW, ACS_RTEE);
    attroff(COLOR_PAIR(6));

    // --- Equipment (Yellow) ---
    row = startY + 11;
    
    attron(COLOR_PAIR(5)); // Yellow
    
    // Weapon
    mvprintw(row, statX, "Weapon: ");
    if (inventory.equippedWeapon) {
        printw("%s", inventory.equippedWeapon->get_item_name().c_str());
    } else {
        attron(A_DIM); // Dim the "None" text
        printw("(None)");
        attroff(A_DIM);
    }

    // Armor
    mvprintw(row + 1, statX, "Armor:  ");
    if (inventory.equippedArmor) {
        printw("%s", inventory.equippedArmor->get_item_name().c_str());
    } else {
        attron(A_DIM);
        printw("(None)");
        attroff(A_DIM);
    }
    
    attroff(COLOR_PAIR(5));
}

void Player::special_move(Character& enemy) 
{
    std::shared_ptr<Weapon> current_weapon = this->inventory.equippedWeapon;

    if (current_weapon)
    {

        if (current_weapon->special)
        {
            current_weapon->special_attack(*this, enemy, this->world);
        }
        else
        {
            this->world.add_log_message("Your " + current_weapon->get_item_name() + " does not have a special move.");
        }
    }
}
int Player::get_x()
{
    return this->coord_x;
}

int Player::get_y()
{
    return this->coord_y;
}

void Player::set_x(int a)
{
    this->coord_x = a;
}

void Player::set_y(int a)
{
    this->coord_y = a;
}

void Player::use_mana(int amount)
{
    if (this->mana < amount)
    {
        this->mana = 0;
    }
}

void Player::add_mana(int amount)
{
    this->mana += amount;

    if (this->mana > this->max_mana)
    {
        this->mana = this->max_mana;
    }
}
void Player::modify_max_mana(int amount)
{
    this->max_mana += amount;
    if (this->mana > this->max_mana)
    {
        this->mana = this->max_mana;
    }
}
void Player::modify_attack(int amount)
{
    this->attackPower += amount;
}
int Player::get_mana() const
{
    return this->mana;
}

int Player::get_max_mana() const
{
    return this->max_mana;
}

int Player::get_attack_power() const
{
    return this->attackPower;
}
void Player::add_health(int amount)
{
    this->health += amount;
    if (this->health > this->maxHealth)
    {
        this->health = this->maxHealth;
    }
}
void Player::modify_max_health(int amount)
{
    this->maxHealth += amount;
    if (this->health > this->maxHealth)
    {
        this->health = this->maxHealth;
    }
}
std::chrono::steady_clock::time_point Player::get_normal_attack_ready() const
{
    return this->normal_attack_ready;
}

std::chrono::steady_clock::time_point Player::get_special_attack_ready() const
{
    return this->special_attack_ready;
}

void Player::set_normal_attack_cooldown(float seconds)
{
    this->normal_attack_ready = std::chrono::steady_clock::now() +
                                std::chrono::microseconds(static_cast<int>(seconds * 1000000));
}

void Player::set_special_attack_cooldown(float seconds)
{
    this->special_attack_ready = std::chrono::steady_clock::now() +
                                 std::chrono::microseconds(static_cast<int>(seconds * 2500000));
}

void Player::update_mana_regen(std::chrono::steady_clock::time_point current_time)
{
    (void)current_time;
}

void Player::setNormalAttackInterval(double sec)
{
    this->normalAttackInterval = sec;
}

double Player::getNormalAttackInterval() const
{
    return this->normalAttackInterval;
}

void Player::setSpecialAttackInterval(double sec)
{
    this->specialAttackInterval = sec;
}

double Player::getSpecialAttackInterval() const
{
    return this->specialAttackInterval;
}

std::string Player::move(int x, int y, Map& map) 
{
    return "Moved to position (" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

int Player::getHPRegenTime()
{
    return regen_hp_time;
}

void Player::setHPRegenTime(int a)
{
    regen_hp_time = a;
}

int Player::getManaRegenTime()
{
    return regen_mana_time;
}

void Player::setManaRegenTime(int a)
{
    regen_mana_time = a;
}

void Player::set_health(int amt)
{
    health = amt;
}

Player &Player::operator=(const Player &other)
{
    if (this == &other)
    {
        return *this;
    }
    Character::operator=(other);

    this->type = other.type;
    this->inventory = other.inventory; 

    this->max_mana = other.max_mana;
    this->mana = other.mana;
    this->baseMaxHealth = other.baseMaxHealth;
    this->baseAttackPower = other.baseAttackPower;
    this->baseMaxMana = other.baseMaxMana;
    this->stamina = other.stamina;

    this->regen_hp_time = other.regen_hp_time;
    this->regen_mana_time = other.regen_mana_time;
    this->specialAttackInterval = other.specialAttackInterval;
    this->normalAttackInterval = other.normalAttackInterval;

    this->normal_attack_ready = other.normal_attack_ready;
    this->special_attack_ready = other.special_attack_ready;
    this->next_mana_regen = other.next_mana_regen;

    return *this;
}