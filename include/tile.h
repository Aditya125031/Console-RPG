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
        Tile(Player& player, vector<bool>& quest, string displayChar = ".", int x=0, int y=0);

        string getMiniMapDisplayChar() ;
        string getMapDisplayChar() ;
        bool getIsWalkable() ;
        Character* getCharacter() ;
        //Item* getItem() const;
        void setCharacter(Character* character);
        //void setItem(Item* item);
        void setMiniMapDisplayChar(string);
        void setMapDisplayChar(string);
        void setIsWalkable(bool);
        void setQuest();
        bool checkQuest();
        void setBounds(bool);
        bool getBounds();
        bool getQuestStatus(vector<bool>& quest);
        void seQuestStatus(bool);
        int get_map_color_pair() ;
        void set_map_color_pair(int) ;
        int get_mini_map_color_pair() ;
        void set_mini_map_color_pair(int) ;
        int get_doQuest();
        void set_doQuest(int);
        

    private:
        string m_miniMapDisplayChar;
        string m_mapDisplayChar;  
        bool m_isWalkable;           
        Character* m_characterOnTile; 
        int requiredQuestCompleted;
        // Item* m_itemOnTile;   
        bool outOfBounds;
        int m_colorPairMiniMap;
        int m_colorPairMap;
        int doQuest;
    };

#endif 