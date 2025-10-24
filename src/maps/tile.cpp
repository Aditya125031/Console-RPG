#include <bits/stdc++.h>
#include "../include/tile.h"
#include "../include/player.h"
#include "../include/colors.h"
using namespace std;


Tile::Tile() {}

// Constructor implementation
Tile::Tile(Player& player, string displayChar, int x, int y)
    : m_isWalkable(true), m_tileCode(0),
      m_characterOnTile(nullptr), outOfBounds(false) // Always initialize pointers to nullptr
    //   ,m_itemOnTile(nullptr), m_reqQuest(nullptr) 
    { 
        if(displayChar=="."){
            //m_miniMapDisplayChar+=Color::BG_GRAY;
            //m_mapDisplayChar+=Color::BG_GRAY;
            m_mapDisplayChar += ".";
            m_miniMapDisplayChar += ".";
            //m_miniMapDisplayChar+=Color::RESET;
            //m_mapDisplayChar+=Color::RESET;
            m_isWalkable = true;
        }
        else if(displayChar=="W"){
            m_miniMapDisplayChar+=Color::FG_BRIGHT_BLUE;
            m_mapDisplayChar+=Color::FG_BRIGHT_BLUE;
            m_mapDisplayChar += "≈";
            m_miniMapDisplayChar += "≈";
            m_miniMapDisplayChar+=Color::RESET;
            m_mapDisplayChar+=Color::RESET;
            m_isWalkable = false;
        }
        else if(displayChar=="T"){
            m_miniMapDisplayChar+=Color::FG_GREEN;
            m_mapDisplayChar+=Color::FG_GREEN;
            m_mapDisplayChar += "♣";
            m_miniMapDisplayChar += "♣";
            m_miniMapDisplayChar+=Color::RESET;
            m_mapDisplayChar+=Color::RESET;
            m_isWalkable = false;
        }
        else if(displayChar=="P"){
            m_isWalkable = true;
            m_miniMapDisplayChar+=Color::FG_YELLOW;
            m_mapDisplayChar+=Color::FG_YELLOW;
            m_mapDisplayChar += "♞";
            m_miniMapDisplayChar += "♞";
            m_miniMapDisplayChar+=Color::RESET;
            m_mapDisplayChar+=Color::RESET;
            Character* playerPtr = &player;
            m_characterOnTile = playerPtr;
            player.set_x(x);
            player.set_y(y);
        }
        else if(displayChar=="G"){
            m_miniMapDisplayChar+=Color::FG_RED;
            m_mapDisplayChar = ".";
            m_miniMapDisplayChar += "§";
            m_miniMapDisplayChar+=Color::RESET;
            Character* goblin = new Goblin("Goblin", 50, 5);
            m_characterOnTile = goblin;
            m_isWalkable = false;
        }
        // else if(displayChar=="QB1"){
        //     m_tileCode = 111;
        //     m_characterOnTile = new Boss();
        //     m_displayChar = 'Ö';
        //     m_reqQuest = new q2();
        //     m_isWalkable = true;
        // }
        else{
            m_mapDisplayChar = "X";
            m_miniMapDisplayChar = "X";
            m_isWalkable = false;
        }

      }

// Getter for the display character
string Tile::getMiniMapDisplayChar() {
    // if(m_reqQuest == nullptr || m_reqQuest->isCompleted()){
    //     return m_miniMapDisplayChar;
    // }
    // else{
    //     return '.';
    // }
    return m_miniMapDisplayChar;
}

string Tile::getMapDisplayChar() {
    return m_mapDisplayChar;
}


// Getter for walkability
bool Tile::getIsWalkable() {
    // A tile is not walkable if it's inherently a wall OR if a character is on it.
    return m_isWalkable;
}

// Getter for the character pointer
Character* Tile::getCharacter() {
    return m_characterOnTile;
}

// Getter for the item pointer
// Item* Tile::getItem() const {
//     return m_itemOnTile;
// }

// Setter for the character pointer
void Tile::setCharacter(Character* character) {
    m_characterOnTile = character;
}

// Setter for the item pointer
// void Tile::setItem(Item* item) {
//     m_itemOnTile = item;
// }

// void Tile::setQuest() {
//     m_reqQuest->isCompleted()=true;
// }

int Tile::getTileCode() {
    return m_tileCode;
}

void Tile::setBounds(bool status) {
    outOfBounds = status;
}

void Tile::setMiniMapDisplayChar(string displayChar) {
    m_miniMapDisplayChar = displayChar;
}

void Tile::setMapDisplayChar(string displayChar) {
    m_mapDisplayChar = displayChar;
}

void Tile::setIsWalkable(bool status) {
    m_isWalkable = status;
}

bool Tile::getBounds() {
    return outOfBounds;
}

void Tile::setTileCode(int code) {
    m_tileCode = code;
}