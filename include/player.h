#ifndef PLAYER_H
#define PLAYER_H

#include "../include/character.h"  
#include <string>
#include <chrono>
using namespace std;

class Map;

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
    int stamina;
    std::chrono::steady_clock::time_point normal_attack_ready;
    std::chrono::steady_clock::time_point special_attack_ready;
    std::chrono::steady_clock::time_point next_mana_regen;

public:
    Player(std::string name, PlayerType type); 
    int get_x() override;
    int get_y() override;
    void set_x(int) override;
    void set_y(int) override;
    void use_mana(int amount);
    void add_mana(int amount);
    void modify_maxmana(int amount);
    void update_mana_regen(std::chrono::steady_clock::time_point current_time);
    int get_mana() const;
    int get_max_mana() const;
    void add_health(int amount);
    void modify_max_health(int amount);
    void modify_max_mana(int amount);
    std::string get_type_string() const;
    Player(std::string name, PlayerType type); 
    int get_x() override;
    int get_y() override;
    void set_x(int) override;
    void set_y(int) override;
    void show_details() const; 
    void special_move(Character& enemy); 
    std::chrono::steady_clock::time_point get_normal_attack_ready() const;
    std::chrono::steady_clock::time_point get_special_attack_ready() const;
    void set_normal_attack_cooldown(float seconds);
    void set_special_attack_cooldown(float seconds);
};

#endif
