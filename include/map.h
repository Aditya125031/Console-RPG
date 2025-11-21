#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include<cmath>
#include <limits>
#include <chrono>
#include <cstdlib> 
#include "tile.h" 
using namespace std;

class Player;

class Map {
public:
    Map();
    Map(Player& player, vector<bool>& quest, int width = 0, int height = 0, string filename = "");

    bool loadFromFile(const string& filename, vector<bool>& quest, Player& player);
    void render();
    int get_minimap_view(Player& player, int view_width, int view_height, deque<string>&);
    Tile* getTileAt(int x, int y);
    int getWidth() const;
    int getHeight() const;

private:
    int m_width;
    int m_height;
    vector<vector<Tile>> m_grid; 
};

#endif 