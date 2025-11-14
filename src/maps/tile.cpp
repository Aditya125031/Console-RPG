#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include<cmath>
#include <limits>
#include <chrono>
#include <cstdlib>
#include "../include/tile.h"
#include "../include/player.h"
#include "../include/war_chief.h"
#include "../include/orc.h"
#include "../include/orc_raider.h"
#include "../include/golem.h"
#include "../include/orc_raider.h"
#include "../include/infernalimp.h"
#include "../include/necromancer.h"
#include "../include/imp.h"
#include "../include/goblin.h"
#include "../include/giant.h"
#include "../include/stoneimp.h"
#include "../include/final_boss.h"
#include "../include/npc.h"
using namespace std;


Tile::Tile() {}

Tile::Tile(Player& player, vector<bool>& quest, string displayChar, int x, int y)
    : m_isWalkable(true), m_characterOnTile(nullptr), outOfBounds(false), requiredQuestCompleted(-1),
        doQuest(-1), isNPC(false)
    //   ,m_itemOnTile(nullptr), 
    { 
        if(displayChar=="."){
            m_mapDisplayChar = m_miniMapDisplayChar = ".";
            m_isWalkable = true;
            m_colorPairMap = m_colorPairMiniMap = 6; 
        }
        else if(displayChar=="W"){
            m_mapDisplayChar = m_miniMapDisplayChar = "≈";
            m_isWalkable = false;
            m_colorPairMap = m_colorPairMiniMap = 3; 
        }
        else if(displayChar=="T"){
            m_mapDisplayChar = m_miniMapDisplayChar = "♣";
            m_isWalkable = false;
            m_colorPairMap = m_colorPairMiniMap = 2; 
        }
        else if(displayChar=="F"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "┼";
            m_isWalkable = false;
            m_colorPairMap = m_colorPairMiniMap = 6; 
        }
        else if(displayChar=="P"){
            m_isWalkable = true;
            m_mapDisplayChar = m_miniMapDisplayChar = "♞";
            m_colorPairMap = m_colorPairMiniMap = 5; 

            Character* playerPtr = &player;
            m_characterOnTile = playerPtr;
            player.set_x(x);
            player.set_y(y);
        }
        else if(displayChar=="G"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "§";
            
            m_colorPairMap = 6;
            m_colorPairMiniMap = 4; 

            Character* goblin = new Goblin();
            m_characterOnTile = goblin;
            m_isWalkable = false;
        }
        else if(displayChar=="I"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "§"; 
            
            m_colorPairMap = 6;
            m_colorPairMiniMap = 4; 

            Character* imp = new Imp();
            m_characterOnTile = imp;
            m_isWalkable = false;
        }
        else if(displayChar=="O"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "§"; 
            
            m_colorPairMap = 6;
            m_colorPairMiniMap = 4; 

            Character* orc = new Orc();
            m_characterOnTile = orc;
            m_isWalkable = false;
        }
        else if(displayChar=="GG"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "Ω"; 
            
            m_colorPairMap = 6;
            m_colorPairMiniMap = 4; 

            Character* gg = new GiantGoblin();
            m_characterOnTile = gg;
            m_isWalkable = false;
        }
        else if(displayChar=="SI"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "Ω"; 
            
            m_colorPairMap = 6;
            m_colorPairMiniMap = 4; 

            Character* simp = new StoneImp();
            m_characterOnTile = simp;
            m_isWalkable = false;
        }
        else if(displayChar=="QGW1"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "Ö";
            m_colorPairMap = 6;
            m_colorPairMiniMap = 4; 
            Character* warchief = new GoblinWarChief();
            m_characterOnTile = warchief;
            m_isWalkable = false;
            doQuest=0;
        }
        else if(displayChar=="QOR2"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "Ö";
            m_colorPairMap = 6;
            m_colorPairMiniMap = 4; 
            Character* orc = new OrcRaider();
            m_characterOnTile = orc;
            requiredQuestCompleted = 0;
            m_isWalkable = false;
            doQuest=1;
        }
        else if(displayChar=="QBGLM1"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "⚜";
            m_colorPairMap = 6;
            m_colorPairMiniMap = 4; 
            Character* golem = new BoneGolem();
            m_characterOnTile = golem;
            requiredQuestCompleted = 1;
            m_isWalkable = false;
            doQuest=2;
        }
        else if(displayChar=="QII3"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "Ö";
            m_colorPairMap = 6;
            m_colorPairMiniMap = 4; 
            Character* infimp = new InfernalImp();
            m_characterOnTile = infimp;
            requiredQuestCompleted = 2;
            m_isWalkable = false;
            doQuest=3;
        }
        else if(displayChar=="QBNM2"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "⚜";
            m_colorPairMap = 6;
            m_colorPairMiniMap = 4; 
            Character* golem = new BoneGolem();
            m_characterOnTile = golem;
            requiredQuestCompleted = 3;
            m_isWalkable = false;
            doQuest=4;
        }
        else if(displayChar=="QFB"){
            m_mapDisplayChar = "."; 
            m_miniMapDisplayChar = "♛";
            m_colorPairMap = 6;
            m_colorPairMiniMap = 5; 
            Character* lichlord = new LichLord();
            m_characterOnTile = lichlord;
            requiredQuestCompleted = 4;
            m_isWalkable = false;
        }
        else if(displayChar=="NPC"){
            m_mapDisplayChar = "!"; 
            m_miniMapDisplayChar = "!";
            isNPC=true;
            m_colorPairMap = m_colorPairMiniMap = 5; 
            Character* npc = new NPC();
            m_characterOnTile = npc;
            m_isWalkable = false;
        }
        else{
            m_mapDisplayChar = m_miniMapDisplayChar = "X";
            m_isWalkable = false;
            m_colorPairMap = m_colorPairMiniMap = 7;
        }

      }

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


bool Tile::getIsWalkable() {
    return m_isWalkable;
}

Character* Tile::getCharacter() {
    return m_characterOnTile;
}

// Getter for the item pointer
// Item* Tile::getItem() const {
//     return m_itemOnTile;
// }

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

int Tile::get_map_color_pair() {
    return m_colorPairMap;
}
int Tile::get_mini_map_color_pair() {
    return m_colorPairMiniMap;
}

void Tile::set_mini_map_color_pair(int a){
    m_colorPairMiniMap=a;
}

void Tile::set_map_color_pair(int a){
    m_colorPairMap=a;
}

void Tile::seQuestStatus(bool status){
    requiredQuestCompleted=status;
}

bool Tile::getQuestStatus(vector<bool>& quest){
    if(requiredQuestCompleted == -1) return true;
    return quest[requiredQuestCompleted];
}

int Tile::get_doQuest(){
    return doQuest;
}

void Tile::set_doQuest(int a){
    doQuest=a;
}

bool Tile::get_isNPC(){
    return isNPC;
}
