// Player.cpp
#include "../include/player.h"
#include "../include/map.h"
#include "../include/tile.h"
#include "../include/colors.h"
#include <iostream> 
using namespace std;

Player::Player(std::string name, PlayerType type)
    : Character(name, 100, 10), type(type) {
    switch (type) {
        case PlayerType::Swordsman:
            this->health = 150;
            this->attackPower = 15;
            this->mana = 20;
            this->stamina = 100;
            break;

        case PlayerType::Archer:
            this->health = 100;
            this->attackPower = 12;
            this->mana = 40;
            this->stamina = 120;
            break;

        case PlayerType::Mage:
            this->health = 80;
            this->attackPower = 8;
            this->mana = 150;
            this->stamina = 50;
            break;
    }
    this->maxHealth = this->health;

    std::cout << "A new " << get_type_string() << " named " << this->name << " has arrived!" << std::endl;
}

string Player::move(int x, int y, Map& map) {
    int newx=coord_x+y;
    int newy=coord_y+x;
    if(map.getTileAt(newx,newy)->getCharacter() != nullptr) {
        return "Combat Triggered!";
        // int k=startCombat(myPlayer, map->getTileAt(newy,newx)->getCharacter());
        // if(k==0){
        //     //end game
        //     cout << "You have been defeated!"<<endl;
        // }
        // else if(k==1){
        //     cout<<"You defeated the enemy!"<<endl;
        //     map->getTileAt(newy,newx)->setisWalkable();
        //     map->getTileAt(coord_y,coord_x)->setCharacter(nullptr);
        //     map->getTileAt(newy,newx)->setCharacter(this);
        //     coord_x=newx;
        //     coord_y=newy;
        // }
        // else if(k==2){
        //     cout<<"You fled the battle!"<<endl;
        // }
    } 
    // else if(map->getTileAt(newy,newx)->getItem()) {
    //     cout << "You found an item!" << endl;
    //     int k=pickItem(myPlayer, map->getTileAt(newy,newx)->getItem());
    //     if(k==1){
    //         cout << "You picked up the item!" << endl;
    //         map->getTileAt(newy,newx)->setItem(nullptr);
    //         map->getTileAt(coord_y,coord_x)->setCharacter(nullptr);
    //         map->getTileAt(newy,newx)->setCharacter(this);
    //         coord_x=newx;
    //         coord_y=newy;
    //     } else {
    //         cout << "You didn't pick up the item!" << endl;
    //     }
    // } 
    else if(map.getTileAt(newx,newy)->getBounds()) {
        return "Do not venture outside the forest!";
    } 
    else if(map.getTileAt(newx,newy)->getIsWalkable()) {
        map.getTileAt(coord_x,coord_y)->setCharacter(nullptr);
        map.getTileAt(newx,newy)->setCharacter(this);
        map.getTileAt(newx,newy)->setMiniMapDisplayChar(Color::FG_YELLOW + "♞" + Color::RESET);
        map.getTileAt(coord_x,coord_y)->setMiniMapDisplayChar(".");
        map.getTileAt(newx,newy)->setMapDisplayChar(Color::FG_YELLOW + "♞" + Color::RESET);
        map.getTileAt(coord_x,coord_y)->setMapDisplayChar(".");
        this->coord_x=newx;
        this->coord_y=newy;
        return "You moved to (" + to_string(coord_y) + ", " + to_string(coord_x) + ").";
    }
    else {
        return "You can't move there!";
    }
    return "Achievement Unlocked! The Void!";
}

std::string Player::get_type_string() const {
    switch (this->type) {
        case PlayerType::Swordsman: return "Swordsman";
        case PlayerType::Archer:    return "Archer";
        case PlayerType::Mage:      return "Mage";
        default:                    return "Unknown";
    }
}

void Player::show_details() const {
    std::cout << "---------------------\n"
              << "Name: " << this->name << "\n"
              << "Type: " << get_type_string() << "\n"
              << "Health: " << this->health << " / " << this->maxHealth << "\n" // Show max health too
              << "Attack Power: " << this->attackPower << "\n"
              << "Mana: " << this->mana << "\n"
              << "Stamina: " << this->stamina << "\n"
              << "---------------------\n";
}

// Note: 'Character' with a capital 'C'
void Player::special_move(Character& enemy) 
{
    std::cout << this->name << " uses their special move: ";
    int damage = 0;
    switch (this->type) {
        case PlayerType::Swordsman:
            if (this->stamina >= 25) {
                std::cout << "'Whirlwind Slash'!" << std::endl;
                damage = this->attackPower * 2.5; // Example: 2.5x damage
                this->stamina -= 25;
            } else {
                std::cout << "Not enough stamina!" << std::endl;
            }
            break;

        case PlayerType::Archer:
            if (this->stamina >= 20) {
                std::cout << "'Piercing Arrow'!" << std::endl;
                damage = this->attackPower * 3; 
                this->stamina -= 20;
            } else {
                std::cout << "Not enough stamina!" << std::endl;
            }
            break;

        case PlayerType::Mage:
            if (this->mana >= 30) {
                std::cout << "'Fireball'!" << std::endl;
                damage = 40;
                this->mana -= 30;
            } else {
                std::cout << "Not enough mana!" << std::endl;
            }
            break;
    }
    if (damage > 0) {
        enemy.take_damage(damage);
    }
}
int Player::get_x() const {
    return this->coord_x;
}

int Player::get_y() const {
    return this->coord_y;
}

void Player::set_x(int a) {
    this->coord_x=a;
}

void Player::set_y(int a){
    this->coord_y=a;
}