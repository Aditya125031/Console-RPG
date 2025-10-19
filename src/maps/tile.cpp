#include <bits/stdc++.h>
#include "../include/tile.h"
#include "../include/character.h" // Include the full definitions here
#include "../include/items.h"
using namespace std;

// Constructor implementation
Tile::Tile(string displayChar)
    : m_isWalkable(true), m_tileCode(0),
      m_characterOnTile(nullptr) // Always initialize pointers to nullptr
    //   ,m_itemOnTile(nullptr), m_reqQuest(nullptr) 
    {
        if(displayChar=="."){
            m_displayChar = '.';
            m_isWalkable = true;
        }
        else if(displayChar=="L"){
            m_displayChar = 'L';
            m_isWalkable = false;
        }
        else if(displayChar=="P"){
            m_isWalkable = true;
            m_displayChar = 'P';
            //m_characterOnTile = new Player();
        }
        else if(displayChar=="G"){
            m_displayChar = 'G';
            //m_characterOnTile = new Goblin();
            m_isWalkable = false;
        }
        // else if(displayChar=="QB1"){
        //     m_tileCode = 111;
        //     m_characterOnTile = new Boss();
        //     m_displayChar = '.';
        //     m_reqQuest = new q2();
        //     m_isWalkable = true;
        // }
        else{
            m_displayChar = '#';
            m_isWalkable = true;
        }

      }

// Getter for the display character
char Tile::getDisplayChar() const {
    // if(m_reqQuest == nullptr || m_reqQuest->isCompleted()){
    //     return m_displayChar;
    // }
    // else{
    //     return '.';
    // }
    return m_displayChar;
}

// Getter for walkability
bool Tile::getIsWalkable() const {
    // A tile is not walkable if it's inherently a wall OR if a character is on it.
    return m_isWalkable;
}

// Getter for the character pointer
Character* Tile::getCharacter() const {
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

void Tile::setDisplayChar() {
    m_displayChar = '.';
}

void Tile::setIsWalkable() {
    m_isWalkable = true;
}

// void Player::move(int x, int y, Map& map) {
//     int newx=curx+x;
//     int newy=cury+y;
//     if(map->getTileAt(newy,newx)->getCharacter() != nullptr) {
//         int k=startCombat(myPlayer, map->getTileAt(newy,newx)->getCharacter());
//         if(k==0){
//             //end game
//             cout << "You have been defeated!"<<endl;
//         }
//         else if(k==1){
//             cout<<"You defeated the enemy!"<<endl;
//             map->getTileAt(newy,newx)->setisWalkable();
//             map->getTileAt(cury,curx)->setCharacter(nullptr);
//             map->getTileAt(newy,newx)->setCharacter(this);
//             curx=newx;
//             cury=newy;
//         }
//         else if(k==2){
//             cout<<"You fled the battle!"<<endl;
//         }
//     } 
//     else if(map->getTileAt(newy,newx)->getItem()) {
//         cout << "You found an item!" << endl;
//         int k=pickItem(myPlayer, map->getTileAt(newy,newx)->getItem());
//         if(k==1){
//             cout << "You picked up the item!" << endl;
//             map->getTileAt(newy,newx)->setItem(nullptr);
//             map->getTileAt(cury,curx)->setCharacter(nullptr);
//             map->getTileAt(newy,newx)->setCharacter(this);
//             curx=newx;
//             cury=newy;
//         } else {
//             cout << "You didn't pick up the item!" << endl;
//         }
//     } 
//     else if(!map->getTileAt(newy,newx)->getBounds()) {
//         cout << "You must not venture outside the forest, the devourer is always looking!" << endl;
//     } 
//     else if(map->getTileAt(newy,newx)->getIsWalkable() && map->getTileAt(newy,newx)->getBounds()) {
//         map->getTileAt(cury,curx)->setCharacter(nullptr);
//         map->getTileAt(newy,newx)->setCharacter(this);
//         curx=newx;
//         cury=newy;
//     }
//     else {
//         cout << "You can't move there!" << endl;
//     }
// }
