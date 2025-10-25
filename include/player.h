#ifndef PLAYER_H
#define PLAYER_H

#include "../include/character.h"  
#include "inventory.hpp"
#include <string>
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
    int mana;
    int stamina;
    int coord_x=15, coord_y=45;

public:
    Inventory inventory;
    std::string get_type_string() const;
    Player(std::string name, PlayerType type);
    string move(int , int , Map&); 
    int get_x() const;
    int get_y() const;
    void set_x(int);
    void set_y(int);
    void show_details() const; 
    void special_move(Character& enemy); 
};

#endif