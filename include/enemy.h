// Enemy.h
#include "character.h"

class enemy : public Character {
public:
    int coord_x,coord_y;
    enemy(std::string name, int health, int attackPower,int x,int y);
    std::string get_name() const;
    int get_x() const;
    int get_y() const;
};