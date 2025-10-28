// In your src/core/ directory
//#include <iostream> // Replaced by curses I/O
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <cstdlib> // Required for std::system (kept for potential fallback)
#include <sstream>

// ⭐️ Include the PDCurses header
#include "../extern/pdcurses/curses.h"

// Your project headers
#include "../include/map.h"
#include "../include/tile.h"
#include "../include/player.h"
#include "../include/colors.h"

using namespace std;

Map::Map() {
    m_width = 0;
    m_height = 0;
}
Map::Map(Player& player, int width, int height, string filename) {
    m_width = width;
    m_height = height;
    // Note: Grid dimensions are typically [row][col], which corresponds to [height][width]
    m_grid.resize(height, vector<Tile>(width)); 
    loadFromFile(filename, player);
}

void Map::get_minimap_view(Player& player, int view_width, int view_height, std::deque<string>& event_log) {
    // --- 1. Get Player Position ---
    int player_x = player.get_x(); // column
    int player_y = player.get_y(); // row

    // --- 2. Calculate Ideal Viewport Start (Centered on Player) ---
    // ⭐️ SWAPPED: y is width, x is height
    int start_y = player_y - view_width / 2;
    int start_x = player_x - view_height / 2;

    // --- 3. Clamp Viewport to Map Boundaries ---

    // Clamp to top-left boundary (0, 0)
    if (start_y < 0) start_y = 0;
    if (start_x < 0) start_x = 0;

    // Clamp to bottom-right boundary
    // ⭐️ SWAPPED: y (width) is clamped against m_width
    if (start_y + view_width > m_width) {
        start_y = m_width - view_width;
    }
    // ⭐️ SWAPPED: x (height) is clamped against m_height
    if (start_x + view_height > m_height) {
        start_x = m_height - view_height;
    }

    // Final re-clamp in case the map is *smaller* than the viewport
    if (start_y < 0) start_y = 0;
    if (start_x < 0) start_x = 0;

    // --- 4. Define Actual Loop Endpoints ---
    // ⭐️ SWAPPED: y (width) with m_width, x (height) with m_height
    int end_y = std::min(start_y + view_width, m_width);
    int end_x = std::min(start_x + view_height, m_height);
    
    // --- 6. Get dimensions and calculate centering ---
    int term_width = 0;
    int term_height = 0;
    getmaxyx(stdscr, term_height, term_width); // Curses function

    // Calculate effective widths (for drawing the border)
    // ⭐️ SWAPPED: minimap width is determined by the y-range (cols)
    int minimap_output_width = (end_y - start_y) * 2; // Width of the characters + spaces
    int map_section_width = minimap_output_width + 1; // +1 for the border space
    
    // The total width of the content box (borders included)
    int content_width = map_section_width + 5; // map + border + log

    // You said the header takes 3 lines (0, 1, 2)
    int top_offset = 3; 
    
    // Calculate vertical and horizontal centering
    // ⭐️ SWAPPED: content_height is determined by the x-range (rows)
    int content_height = end_x - start_x + 2; // +2 for top/bottom borders
    int top_start_row = top_offset + std::max(0, (term_height - top_offset - content_height) / 2);
    int left_start_col = std::max(0, (term_width - content_width) / 2);

    // --- 7. Build Borders and Separators ---
    std::string single_char = "─"; // Assuming this is the character you want
    std::string h_line_map;
    for (int i = 0; i < map_section_width; ++i) {
        h_line_map += single_char;
    }
    
    std::string border_top    = "┌" + h_line_map + "┬" +  "─";
    std::string border_bottom = "└" + h_line_map + "┴" +  "─";
    
    // --- 8. Print the centered content using Curses ---
    int current_row = top_start_row;

    // 1. Print the top border
    mvprintw(current_row++, left_start_col, "%s", border_top.c_str());

    // 2. Print the map content and event log
    // ⭐️ SWAPPED: Outer loop is 'x' (rows/height)
    for (int x = start_x, row_index = 0; x < end_x; ++x, ++row_index) {
        
        // Use move() to set the cursor to the correct starting column for *this* line
        move(current_row, left_start_col);
        
        addstr("│ "); // Print left map border
        
        // ⭐️ SWAPPED: Inner loop is 'y' (cols/width)
        for (int y = start_y; y < end_y; ++y) {
            
            // ⭐️ SWAPPED: Array access is m_grid[x][y]
            int clrCode=m_grid[x][y].get_mini_map_color_pair();             
            attron(COLOR_PAIR(clrCode));
            addstr((m_grid[x][y].getMiniMapDisplayChar()+" ").c_str());    
            attroff(COLOR_PAIR(clrCode));
        }
        addstr("│ "); // Print right map border / log separator

        // Get the corresponding log line
        if (row_index < event_log.size()) {
            attron(A_DIM | COLOR_PAIR(6));          // Set color (White)
            addstr(event_log[row_index].c_str());      // Log content
            attroff(A_DIM | COLOR_PAIR(6));
        }
        
        // Manually increment the row counter.
        current_row++;
    }

    // 3. Print the bottom border
    mvprintw(current_row++, left_start_col, "%s", border_bottom.c_str());
}

bool Map::loadFromFile(const string& filename, Player& player) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        // ⭐️ REPLACED: cerr with mvprintw
        mvprintw(getmaxy(stdscr) - 1, 0, "Error: Could not open map file: %s", filename.c_str());
        refresh();
        getch();
        return false;
    }
    
    // ... (rest of loadFromFile remains standard C++ logic)

    // Read the map line by line from the file
    string line;
    int row = 0;

    while (getline(inputFile, line) && row < m_height) {
        int col = 0; // Reset the column for each new line
        for (int i = 0; i < line.length() && col < m_grid[row].size(); i++) {

            if (line[i] == '{') {
                string tileChar = "";
                i++; // Move past the '{'
                while (i < line.length() && line[i] != '}') {
                    tileChar += line[i];
                    i++;
                }
                // Your map is indexed as m_grid[row][col]
                m_grid[row][col] = Tile(player, tileChar, row, col); 
            } else {
                string tileChar = "";
                tileChar += line[i];
                m_grid[row][col] = Tile(player, tileChar, row, col);
            }
            if(row==0 || row==m_height-1 || col==0 || col==m_width-1){
                m_grid[row][col].setBounds(true);
                m_grid[row][col].setIsWalkable(false);
            }
            col++; // Move to the next column in our map
        }
        row++; // Move to the next row after the line is processed
    }

    inputFile.close();
    return true;
}

void Map::render() { 
    clear(); 
    int term_width, term_height;
    getmaxyx(stdscr, term_height, term_width); // Curses get dimensions
    std::string h_line;
    std::string single_char = "═"; // Assuming this is the character you want
    // Create the horizontal border line
    for (int i = 0; i < m_width; ++i) {
        h_line += single_char;
    }

    // --- 3. Print the complete map using Curses ---
    int map_display_width = m_width + 2;   // ✅ CORRECT
    int map_display_height = m_height + 2; // ✅ CORRECT

    // Calculate centering (optional, but good practice)
    int start_row = 0;         //std::max(0, (term_height - map_display_height) / 2);
    float start_col = 0.5;     //std::max(0.5, (term_width - map_display_width) / 2);
    
    // We print the pre-built string line by line
    int current_row = start_row;

    // Add the Top Border
    string top_border = "╔" + h_line + "╗";
    mvprintw(current_row++, start_col, "%s", top_border.c_str());

    // Add the Map Rows with Vertical Borders
    for (int y = 0; y < m_height; ++y) {
        move(current_row, start_col);
        
        // ⭐️ FIX: Removed left_pad_str. We are already at the correct column.
        addstr("║"); // Print left map border
        
        for (int x = 0; x < m_width; ++x) {
            int clrCode=m_grid[y][x].get_map_color_pair();                    
            attron(COLOR_PAIR(clrCode));
            addstr((m_grid[y][x].getMapDisplayChar()).c_str());    
            attroff(COLOR_PAIR(clrCode));
        }
        
        addstr("║"); // Print right map border
        current_row++;
    }

    // Add the Bottom Border
    string bottom_border = "╚" + h_line + "╝" ;
    mvprintw(current_row++, start_col, "%s", bottom_border.c_str());
    mvprintw(current_row, 0, "\n(Press 'M' or 'Esc' to close)");
    refresh(); // Show changes
}

Tile* Map::getTileAt(int x, int y) {
    if (x >= 0 && x < m_height && y >= 0 && y < m_width) {
        return &m_grid[x][y];
    }
    return nullptr;
}

int Map::getWidth() const {
    return m_width;
}

int Map::getHeight() const {
    return m_height;
}