#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include<cmath>
#include <limits>
#include <chrono>
#include <cstdlib> /
#include "../include/character.h"
#include "../include/items.h"
using namespace std;

class Player;
class Goblin;
class Inventory;
class Tile{
    public:
        Tile();
        Tile(Player& player, vector<bool>& quest, string displayChar = ".", int x=0, int y=0);

        string getMiniMapDisplayChar() ;
        string getMapDisplayChar() ;
        bool getIsWalkable() ;
        Character* getCharacter() ;
        void setCharacter(Character* character);
        void setMiniMapDisplayChar(string);
        void setMapDisplayChar(string);
        void setIsWalkable(bool);
        void setQuest();
        bool checkQuest();
        void setBounds(bool);
        bool getBounds();
        bool getQuestStatus(vector<bool>& quest);
        void setQuestStatus(bool);
        int get_map_color_pair() ;
        void set_map_color_pair(int) ;
        int get_mini_map_color_pair() ;
        void set_mini_map_color_pair(int) ;
        int get_doQuest();
        void set_doQuest(int);
        int getRequiredQuestCompleted();
        void setRequiredQuestCompleted(int);
        bool get_isNPC();
        vector<std::shared_ptr<Item>>* getLootOnTile();
        void setLootOnTile(vector<std::shared_ptr<Item>>* loot);
        

    private:
        string m_miniMapDisplayChar;
        string m_mapDisplayChar;  
        bool m_isWalkable;           
        Character* m_characterOnTile; 
        int requiredQuestCompleted;
        vector<std::shared_ptr<Item>>* m_lootOnTile;   
        bool outOfBounds;
        int m_colorPairMiniMap;
        int m_colorPairMap;
        int doQuest;
        bool isNPC;
    };

#endif