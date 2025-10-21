// In your src/core/ directory
#include <bits/stdc++.h>
#include "../include/map.h"
#include "../include/tile.h"
using namespace std;


Map::Map(int width, int height, string filename){
    m_width = width;
    m_height = height;
    m_grid.resize(height, vector<Tile>(width));
    loadFromFile(filename);
}
std::vector<std::string> Map::get_minimap_view(Player& player, int view_width, int view_height) {
    std::vector<std::string> view;

    // Calculate the top-left corner of the minimap relative to the main map
    int start_x = player.get_x() - view_width / 2;
    int start_y = player.get_y() - view_height / 2;

    // Ensure the view doesn't go out of the map's bounds
    // CORRECTED: Used member variables m_width and m_height
    start_x = std::max(0, std::min(start_x, m_width - view_width));
    start_y = std::max(0, std::min(start_y, m_height - view_height));

    for (int y = 0; y < view_height; ++y) {
        std::string row_string = "";
        for (int x = 0; x < view_width; ++x) {
            // Check if the current coordinate is the player's position
            if ((start_x + x == player.get_x()) && (start_y + y == player.get_y())) {
                row_string += " @"; // Draw player
            } else {
                // Otherwise, draw the map tile
                row_string += " ";
                // CORRECTED: Used m_grid and the tile's getDisplayChar() method
                row_string += m_grid[start_y + y][start_x + x].getDisplayChar();
            }
        }
        view.push_back(row_string);
    }
    return view;
}
bool Map::loadFromFile(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open map file: " << filename << endl;
        return false;
    }
    
    // Read the map line by line from the file
    string line;
    int row = 0;

    while (getline(inputFile, line) && row < m_grid.size()) {
        int col = 0; // Reset the column for each new line
        for (int i = 0; i < line.length() && col < m_grid[row].size(); i++) {
            if (line[i] == '{') {
                string tileChar = "";
                i++; // Move past the '{'
                while (i < line.length() && line[i] != '}') {
                    tileChar += line[i];
                    i++;
                }
                m_grid[row][col] = Tile(tileChar);
            } else {
                string tileChar = "";
                tileChar += line[i];
                m_grid[row][col] = Tile(tileChar);
            }
            col++; // Move to the next column in our map
        }
        row++; // Move to the next row after the line is processed
    }

    inputFile.close();
    return true;
}

void Map::render() const {
    // Clear the console before drawing (platform-specific)
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            // Use the Tile's own display method
            cout << m_grid[y][x].getDisplayChar();
        }
        cout << endl;
    }
}

Tile* Map::getTileAt(int x, int y) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        return &m_grid[y][x];
    }
    return nullptr; // Return null if coordinates are out of bounds
}

int Map::getWidth() const {
    return m_width;
}

int Map::getHeight() const {
    return m_height;
}