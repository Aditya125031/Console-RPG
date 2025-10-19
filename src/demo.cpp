#include<bits/stdc++.h>
#include"../include/tile.h"
#include"../include/character.h"
#include <typeinfo>

using namespace std;

void fabricateMap(const std::string& filename, vector<vector<Tile>>& map) {
    // 1. Create an input file stream object
    std::ifstream inputFile(filename);

    // 2. Check if the file was successfully opened
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return;
    }

    string line;
    int row = 0;

    while (std::getline(inputFile, line) && row < map.size()) {
        int col = 0; // Reset the column for each new line
        for (int i = 0; i < line.length() && col < map[row].size(); i++) {
            if (line[i] == '{') {
                string tileChar = "";
                i++; // Move past the '{'
                while (i < line.length() && line[i] != '}') {
                    tileChar += line[i];
                    i++;
                }
                map[row][col] = Tile(tileChar);
            } else {
                string tileChar = "";
                tileChar += line[i];
                map[row][col] = Tile(tileChar);
            }
            col++; // Move to the next column in our map
        }
        row++; // Move to the next row after the line is processed
    }

    // 4. The file is automatically closed when 'inputFile' goes out of scope.
}

int main() {
    // Assume you have a file named "story.txt" in the same directory.
    vector<vector<Tile>> map(40, vector<Tile>(156));
    fabricateMap("data/map.txt",map);
    
    return 0;
}