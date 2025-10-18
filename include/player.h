
#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h" 
#include <string>

enum class PlayerType {
    Swordsman,
    Archer,
    Mage
};

class Player : public Character {
private:
    PlayerType type;
    int mana;
    int stamina;
    std::string get_type_string() const;
public:
    Player(std::string name, PlayerType type);

    void show_details() const; 

    void special_move();
};

#endif 