#ifndef PLAYER_H
#define PLAYER_H

#include "../include/character.h"  
#include"../include/inventory.hpp"
#include <string>
#include <chrono>
using namespace std;

class Map;
class Game;
enum class PlayerType {
    Swordsman,
    Archer,
    Mage
};

class Player : public Character {
private:
    PlayerType type;
protected:
    int max_mana;
    int mana;
    int baseMaxHealth;
    int baseAttackPower;
    int baseMaxMana;
    std::chrono::steady_clock::time_point normal_attack_ready;
    std::chrono::steady_clock::time_point special_attack_ready;
    std::chrono::steady_clock::time_point next_mana_regen;

public:
Inventory inventory;
    Player(std::string name, PlayerType type); 
    void special_move(Character& enemy); 
    std::string get_type_string() const;
    void show_details() const; 
    int get_x() override;
    int get_y() override;
    void set_x(int) override;
    void set_y(int) override;
    void use_mana(int amount);
    void add_mana(int amount);
    void modify_max_mana(int amount);
    void modify_attack(int amount);
    void reset_stats();
    int get_mana() const;
    int get_max_mana() const;
    int get_attack_power() const;
    void add_health(int amount);
    void modify_max_health(int amount);
    string move(int , int , Map& map); 
    void update_mana_regen(std::chrono::steady_clock::time_point current_time);
    std::chrono::steady_clock::time_point get_normal_attack_ready() const;
    std::chrono::steady_clock::time_point get_special_attack_ready() const;
    void set_normal_attack_cooldown(float seconds);
    void set_special_attack_cooldown(float seconds);
};

#endif