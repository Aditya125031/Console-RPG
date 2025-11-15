#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <cstdlib> 
#include <sstream>

#include "../extern/pdcurses/curses.h"
#include "../include/map.h"
#include "../include/tile.h"
#include "../include/player.h"

using namespace std;

Map::Map() {
    m_width = 0;
    m_height = 0;
}
Map::Map(Player& player,vector<bool>& quest, int width, int height, string filename) {
    m_width = width;
    m_height = height;
    m_grid.resize(height, vector<Tile>(width)); 
    loadFromFile(filename, quest, player);
}

int Map::get_minimap_view(Player& player, int view_width, int view_height, deque<string>& event_log) {
    int player_x = player.get_x();
    int player_y = player.get_y(); 

    int start_y = player_y - view_width / 2;
    int start_x = player_x - view_height / 2;

    if (start_y < 0) start_y = 0;
    if (start_x < 0) start_x = 0;

    if (start_y + view_width > m_width) {
        start_y = m_width - view_width;
    }
    if (start_x + view_height > m_height) {
        start_x = m_height - view_height;
    }

    if (start_y < 0) start_y = 0;
    if (start_x < 0) start_x = 0;

    int end_y = min(start_y + view_width, m_width);
    int end_x = min(start_x + view_height, m_height);
    
    int num_rows = end_x - start_x;
    int map_content_width = (end_y - start_y) * 2; 
    
    int log_content_width = 0;
    for (int i = 0; i < min((int)event_log.size(), num_rows); ++i) {
        if (event_log[i].length() > log_content_width) {
            log_content_width = event_log[i].length();
        }
    }
    if (log_content_width == 0) log_content_width = 10; 

    int map_section_width = map_content_width + 2;
    int log_section_width = log_content_width + 2;

    string h_line_map;
    string h_line_log;
    for (int i = 0; i < map_section_width; ++i) h_line_map += "═";
    for (int i = 0; i < log_section_width; ++i) h_line_log += "═";

    string border_top = "╔" + h_line_map + "╦" + h_line_log + "╗";
    string border_bottom = "╚" + h_line_map + "╩" + h_line_log + "╝";

    int term_width = 0;
    int term_height = 0;
    getmaxyx(stdscr, term_height, term_width); 

    int total_visual_width = map_section_width + 1; 
    int content_height = num_rows + 2; 
    int total_visual_game_width = map_section_width + 1 + log_section_width + 2;
    int top_offset = 1; 
    
    int top_start_row = top_offset + max(0, (term_height - top_offset - content_height) / 2);
    int left_start_col = max(0, (term_width - total_visual_width) / 2) - 2;

    int current_row = top_start_row;

    mvprintw(current_row++, left_start_col, "%s", border_top.c_str());

    for (int x = start_x, row_index = 0; x < end_x; ++x, ++row_index) {
        move(current_row, left_start_col);
        addstr("║ ");
        for (int y = start_y; y < end_y; ++y) {
            int clrCode = m_grid[x][y].get_mini_map_color_pair();
            if(m_grid[x][y].get_isNPC()){
                attron(COLOR_PAIR(clrCode) | A_BOLD);
                addstr((m_grid[x][y].getMiniMapDisplayChar() + " ").c_str());
                attroff(COLOR_PAIR(clrCode) | A_BOLD);
                continue;
            }
            attron(COLOR_PAIR(clrCode));
            addstr((m_grid[x][y].getMiniMapDisplayChar() + " ").c_str());
            attroff(COLOR_PAIR(clrCode));
        }
        addstr(" ║ "); 

        string log_line = (row_index < event_log.size()) ? event_log[row_index] : "";
        
        attron(A_DIM | COLOR_PAIR(6)); 
        printw(" %-*.*s ", log_content_width, log_content_width, log_line.c_str()); 
        
        attroff(A_DIM | COLOR_PAIR(6));
        current_row++;
    }

    mvprintw(current_row++, left_start_col, "%s", border_bottom.c_str());
    return total_visual_game_width;
}

bool Map::loadFromFile(const string& filename, vector<bool>& quest, Player& player) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        mvprintw(getmaxy(stdscr) - 1, 0, "Error: Could not open map file: %s", filename.c_str());
        refresh();
        getch();
        return false;
    }
    
    string line;
    int row = 0;

    while (getline(inputFile, line) && row < m_height) {
        int col = 0;
        for (int i = 0; i < line.length() && col < m_grid[row].size(); i++) {

            if (line[i] == '{') {
                string tileChar = "";
                i++;
                while (i < line.length() && line[i] != '}') {
                    tileChar += line[i];
                    i++;
                }
                m_grid[row][col] = Tile(player, quest, tileChar, row, col); 
            } else {
                string tileChar = "";
                tileChar += line[i];
                m_grid[row][col] = Tile(player, quest, tileChar, row, col);
            }
            if(row==0 || row==m_height-1 || col==0 || col==m_width-1){
                m_grid[row][col].setBounds(true);
                m_grid[row][col].setIsWalkable(false);
            }
            col++;
        }
        row++;
    }

    inputFile.close();
    return true;
}

void Map::render() { 
    clear(); 
    int term_width, term_height;
    getmaxyx(stdscr, term_height, term_width);
    string h_line;
    string single_char = "═";
    for (int i = 0; i < m_width; ++i) {
        h_line += single_char;
    }

    int map_display_width = m_width + 2; 
    int map_display_height = m_height + 2;

    int start_row = 0;   
    float start_col = 0.5;  
    
    int current_row = start_row;

    string top_border = "╔" + h_line + "╗";
    mvprintw(current_row++, start_col, "%s", top_border.c_str());

    for (int y = 0; y < m_height; ++y) {
        move(current_row, start_col);
        addstr("║");
        
        for (int x = 0; x < m_width; ++x) {
            int clrCode=m_grid[y][x].get_map_color_pair();
            if(m_grid[y][x].get_isNPC()){
                attron(COLOR_PAIR(clrCode) | A_BOLD);
                addstr((m_grid[y][x].getMapDisplayChar()).c_str());    
                attroff(COLOR_PAIR(clrCode) | A_BOLD);
                continue;
            }
            attron(COLOR_PAIR(clrCode));
            addstr((m_grid[y][x].getMapDisplayChar()).c_str());    
            attroff(COLOR_PAIR(clrCode));
            
        }
        
        addstr("║");
        current_row++;
    }

    string bottom_border = "╚" + h_line + "╝" ;
    mvprintw(current_row++, start_col, "%s", bottom_border.c_str());
    mvprintw(current_row, 0, "\n(Press 'M' or 'Esc' to close)");
    refresh();
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