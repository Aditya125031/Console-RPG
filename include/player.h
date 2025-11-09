#ifndef PLAYER_H
#define PLAYER_H

#include "../include/character.h"  
#include"../include/inventory.hpp"
#include <string>
#include <chrono>
using namespace std;

class Item;
class Map;
class Game;
class Inventory;
enum class PlayerType {
    Swordsman,
    Archer,
    Mage
};

class Player : public Character {
private:
    Game& world;
    PlayerType type;
    double specialAttackInterval;
    double normalAttackInterval;
protected:
    int max_mana;
    int mana;
    int baseMaxHealth;
    int baseAttackPower;
    int baseMaxMana;
    int stamina;
    std::chrono::steady_clock::time_point normal_attack_ready;
    std::chrono::steady_clock::time_point special_attack_ready;
    std::chrono::steady_clock::time_point next_mana_regen;

public:
Inventory inventory;
    Player(Game& game_world, std::string name, PlayerType type);
    int get_x() override;
    int get_y() override;
    void set_x(int) override;
    void set_y(int) override;
    void use_mana(int amount);
    void add_mana(int amount);
    void update_mana_regen(std::chrono::steady_clock::time_point current_time);
    int get_mana() const;
    int get_max_mana() const;
    void add_health(int amount);
    void modify_max_health(int amount);
    void modify_max_mana(int amount);
    int get_attack_power() const;
    void modify_attack(int amount);
    std::string get_type_string() const;
    string move(int , int , Map& map); 
    void show_details() const; 
    void special_move(Character& enemy);
    void reset_stats();
    std::chrono::steady_clock::time_point get_normal_attack_ready() const;
    std::chrono::steady_clock::time_point get_special_attack_ready() const;
    
    void set_normal_attack_cooldown(float seconds);
    void set_special_attack_cooldown(float seconds);
    
    void setSpecialAttackInterval(double seconds);
    void setNormalAttackInterval(double seconds);
    double getSpecialAttackInterval() const;
    double getNormalAttackInterval() const;
};

#endif