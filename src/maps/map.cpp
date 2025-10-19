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