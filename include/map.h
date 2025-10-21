// In your include/core/ directory
#ifndef MAP_H
#define MAP_H

#include <bits/stdc++.h>
#include "tile.h" // Include your Tile class definition
#include "player.h"
using namespace std;

class Map {
public:
    // ## Constructor ##
    Map(int width = 0, int height = 0, string filename = "");

    // ## Core Functionality ##
    // Loads a map layout from a specified text file. Returns false if it fails.
    bool loadFromFile(const string& filename);

    // Renders the current map state to the console.
    void render() const;
    std::vector<std::string> get_minimap_view(Player& player, int view_width, int view_height);
    // ## Getters ##
    // Safely gets a pointer to the tile at a specific coordinate.
    Tile* getTileAt(int x, int y);
    int getWidth() const;
    int getHeight() const;

private:
    // ## Attributes ##
    int m_width;
    int m_height;
    
    // A vector of vectors is a modern, safe C++ way to create a 2D grid.
    vector<vector<Tile>> m_grid; 
};

#endif // MAP_H