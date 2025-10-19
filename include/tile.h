// tile.h

#ifndef TILE_H
#define TILE_H

#include <bits/stdc++.h>
#include "character.h"
#include "items.h"
using namespace std;

class Tile{
    public:
        // ## Constructor ##
        // Initializes a tile with its visual representation and walkability.
        Tile(string displayChar = ".");

        // ## Getters (Accessors) ##
        // These methods allow other parts of the code to safely read the tile's state.
        char getDisplayChar() const;
        bool getIsWalkable() const;
        Character* getCharacter() const;
        //Item* getItem() const;

        // ## Setters (Mutators) ##
        // These methods allow controlled modification of the tile's state.
        void setCharacter(Character* character);
        //void setItem(Item* item);
        void setDisplayChar();
        void setIsWalkable();
        void setQuest();
        bool checkQuest();
        int getTileCode();
        bool getIsWalkable();
        void setBounds(bool status);
        bool getBounds();

    private:
        // ## Attributes ##
        // These are kept private to enforce encapsulation. They can only be
        // accessed or modified through the public getter and setter methods.
        char m_displayChar;          // The character for rendering (e.g., '.', '#')
        bool m_isWalkable;           // Can a character move onto this tile?
        Character* m_characterOnTile; // Pointer to a Character on this tile (nullptr if none)
        // Item* m_itemOnTile;          // Pointer to an Item on this tile (nullptr if none)
        // Quest* m_reqQuest;
        int m_tileCode;
        bool outOfBounds;

    };

#endif // TILE_H