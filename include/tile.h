// tile.h

#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include<cmath>
#include <limits>
#include <chrono>
#include <cstdlib> // Required for std::system
#include "../include/character.h"
#include "../include/enemy.h"
#include "../include/goblin.h"
#include "../include/items.h"
using namespace std;

class Player;

class Tile{
    public:
        Tile();
        // ## Constructor ##
        // Initializes a tile with its visual representation and walkability.
        Tile(Player& player, string displayChar = ".", int x=0, int y=0);

        // ## Getters (Accessors) ##
        // These methods allow other parts of the code to safely read the tile's state.
        string getMiniMapDisplayChar() ;
        string getMapDisplayChar() ;
        bool getIsWalkable() ;
        Character* getCharacter() ;
        //Item* getItem() const;

        // ## Setters (Mutators) ##
        // These methods allow controlled modification of the tile's state.
        void setCharacter(Character* character);
        //void setItem(Item* item);
        void setMiniMapDisplayChar(string);
        void setMapDisplayChar(string);
        void setIsWalkable(bool);
        void setQuest();
        bool checkQuest();
        int getTileCode();
        void setTileCode(int);
        void setBounds(bool);
        bool getBounds();
        int get_map_color_pair() ;
        void set_map_color_pair(int) ;
        int get_mini_map_color_pair() ;
        void set_mini_map_color_pair(int) ;


    private:
        // ## Attributes #
        // These are kept private to enforce encapsulation. They can only be
        // accessed or modified through the public getter and setter methods.
        string m_miniMapDisplayChar;          // The character for rendering (e.g., '.', '#')
        string m_mapDisplayChar;  
        bool m_isWalkable;           // Can a character move onto this tile?
        Character* m_characterOnTile; // Pointer to a Character on this tile (nullptr if none)
        // Item* m_itemOnTile;          // Pointer to an Item on this tile (nullptr if none)
        // Quest* m_reqQuest;
        int m_tileCode;
        bool outOfBounds;
        int m_colorPairMiniMap;
        int m_colorPairMap;
    };

#endif // TILE_H