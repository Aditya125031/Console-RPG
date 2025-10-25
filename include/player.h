#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h" 
#include "inventory.hpp"
#include <string>

enum class PlayerType {
    Swordsman,
    Archer,
    Mage
};

class Player : public Character {
private:
    PlayerType type;
protected:
    int mana;
    int stamina;
public:
    Inventory inventory;
    int coord_x=45, coord_y=45;
    std::string get_type_string() const;
    Player(std::string name, PlayerType type);
    void move(int dx, int dy); 
    int get_x() const;
    int get_y() const;
    void show_details() const; 
    void special_move(Character& enemy); 
};

#endif