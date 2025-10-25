#ifndef PLAYER_H
#define PLAYER_H

#include "../include/character.h"  
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

public:
    std::string get_type_string() const;
    Player(std::string name, PlayerType type); 
    int get_x() override;
    int get_y() override;
    void set_x(int) override;
    void set_y(int) override;
    void show_details() const; 
    void special_move(Character& enemy); 
};

#endif