#ifndef PLAYER_H
#define PLAYER_H

#include "../include/character.h"
#include <string>
#include <chrono>

// Remove using namespace std; from headers to avoid pollution
class Map;

enum class PlayerType {
    Swordsman,
    Archer,
    Mage
};

class Player : public Character {
private:
    PlayerType type;
    double normalAttackInterval;
    double specialAttackInterval;

protected:
    int max_mana;
    int mana;
    int stamina;

public:
    Player(std::string name, PlayerType type);
    
    // Position overrides
    int get_x() override;
    int get_y() override;
    void set_x(int) override;
    void set_y(int) override;
    
    // Mana management
    void use_mana(int amount);
    void add_mana(int amount);
    void modify_maxmana(int amount);
    void update_mana_regen(std::chrono::steady_clock::time_point current_time);
    int get_mana() const;
    int get_max_mana() const;
    
    // Health management
    void add_health(int amount);
    void modify_max_health(int amount);
    void modify_max_mana(int amount);
    
    // Character info
    std::string get_type_string() const;
    std::string move(int x, int y, Map& map);
    void show_details() const;
    
    // Combat abilities
    void special_move(Character& enemy);
    
    // Attack interval management
    void setNormalAttackInterval(double sec);
    double getNormalAttackInterval() const;
    void setSpecialAttackInterval(double sec);
    double getSpecialAttackInterval() const;
};

#endif // PLAYER_H