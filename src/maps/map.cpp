// In your src/core/ directory
#include <bits/stdc++.h>
#include "../include/map.h"
#include "../include/tile.h"
#include "../include/player.h"
#include "../include/colors.h"
#include <windows.h>
using namespace std;

void getTerminalDimensions(int& width, int& height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

Map::Map() {
    m_width = 0;
    m_height = 0;
}
Map::Map(Player& player, int width, int height, string filename) {
    m_width = width;
    m_height = height;
    m_grid.resize(height, vector<Tile>(width));
    loadFromFile(filename, player);
}

void Map::get_minimap_view(Player& player, int view_width, int view_height, std::deque<string>& event_log) {
        
    // --- 1. Get Player Position ---
    int player_x = player.get_x();
    int player_y = player.get_y();

    // --- 2. Calculate Ideal Viewport Start (Centered on Player) ---
    int start_x = player_x - view_height / 2;
    int start_y = player_y - view_width / 2;

    // --- 3. Clamp Viewport to Map Boundaries ---
    
    // Clamp to top-left boundary (0, 0)
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;

    // Clamp to bottom-right boundary
    // If the view *ends* past the map width, pull the *start* back.
    if (start_x + view_height > m_height) {
        start_x = m_height - view_height;
    }
    if (start_y + view_width > m_width) {
        start_y = m_width - view_width;
    }

    // // Final re-clamp in case the map is *smaller* than the viewport
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;

    // --- 4. Define Actual Loop Endpoints ---
    // These are the real boundaries to draw, ensuring we don't
    // go past the map's actual size.
    int end_y = std::min(start_y + view_width, m_width);
    int end_x = std::min(start_x + view_height, m_height);

    // --- 5. Build the Minimap String ---
    // It's faster to build one string and print it, 
    // rather than calling std::cout for every single character.
    std::string minimap_output;
    // Pre-allocate memory to avoid multiple reallocations
    minimap_output.reserve(view_width * view_height + view_height); 
    // cout << start_x << " " << end_x << endl;
    // cout << start_y << " " << end_y << endl;
    // --- 1. Define content variables ---
    std::vector<std::string> map_lines;
    std::vector<std::string> log_lines;
    size_t max_log_width = 0;
    int minimap_width = 2*(end_y - start_y);

    // --- 2. Build the content (but don't print) ---
    // This loop just gathers the content and finds the max log width
    for (int x = start_x, i = 0; x < end_x; ++x, ++i) {
        std::string map_row = "";
        for (int y = start_y; y < end_y; ++y) {
            map_row += m_grid[x][y].getMiniMapDisplayChar()+" ";
        }
        map_lines.push_back(map_row);

        // Get the corresponding log line
        std::string log_line = "";
        if (i < event_log.size()) {
            log_line = event_log[i];
        }
        log_lines.push_back(log_line);

        // Track the widest log line for padding calculations
        if (log_line.length() > max_log_width) {
            max_log_width = log_line.length();
        }
    }

    // --- 3. Get dimensions and calculate padding ---
    int term_width = 0;
    int term_height = 0;
    getTerminalDimensions(term_width, term_height);

    // Calculate the total dimensions of your minimap+log block
    int content_height = map_lines.size();
    int content_width = minimap_width + 1; // map_width + "|" + log_width

    // You said you print 3 lines at the top already
    int top_offset = 3; 

    // Calculate padding
    // (Total height - lines already used - content height) / 2
    int top_padding = top_offset + std::max(0, (term_height - top_offset - content_height) / 2);
    int left_padding = std::max(0, (term_width - content_width) / 2);
    left_padding = std::max(0, (term_width - content_width) / 2);
    std::string left_pad_str(left_padding, ' '); // <-- Keep this one

    // --- 4. Print the centered content with a border ---

    // Define the widths for the internal sections, adding padding spaces
    int map_section_width = minimap_width + 2; // " map "
    int log_section_width = max_log_width + 2; // " log "

    // --- FIX IS HERE ---
    // We must build the horizontal lines by repeating the "─" STRING, not the '─' CHAR.
    std::string h_line_map = "";
    std::string h_line_log = "";
    for (int i = 0; i < map_section_width; ++i) {
        h_line_map += "─";
    }
    for (int i = 0; i < log_section_width; ++i) {
        h_line_log += "─";
    }

    // Now build the borders with the new strings
    std::string border_top =  "┌" + h_line_map + "┬"  + "─";
    std::string border_bottom = "└" + h_line_map + "┴" + "─";
    // --- END OF FIX ---

    // The total content width is now the width of our border string
    content_width = border_top.length();
    
    // Recalculate left_padding (THIS IS THE ONLY DECLARATION YOU SHOULD HAVE)
    // left_padding = std::max(0, (term_width - content_width) / 2);
    
    
    // --- Start Printing ---

    // Print newlines to vertically center the whole box
    for (int i = 0; i < (top_padding - top_offset); ++i) {
        std::cout << '\n';
    }

    // 1. Print the top border
    std::cout << left_pad_str << border_top << '\n';

    // 2. Print the content rows
    for (size_t i = 0; i < content_height; ++i) {
        // Pad the log message so the right border is straight
        std::string padded_log = log_lines[i] + std::string(max_log_width - log_lines[i].length(), ' ');

        std::cout << left_pad_str      // Left padding
                  << "│ "              // Left border and a space
                  << map_lines[i]      // Map content
                  << " │ "             // Separator border and spaces
                  << Color::FG_GRAY    // Log color
                  << padded_log        // Padded log content
                  << Color::RESET      // Reset color
                  << '\n';             // Newline
    }

    // 3. Print the bottom border
    std::cout << left_pad_str << border_bottom << '\n';
}

bool Map::loadFromFile(const string& filename, Player& player) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open map file: " << filename << endl;
        return false;
    }
    
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
    // Clear the console before drawing
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    std::string map_output;
    // Pre-allocate memory, adding room for borders and newlines
    map_output.reserve((m_width + 3) * (m_height + 2)); 

    // --- 1. Create the horizontal border line ---
    // We must build this as a string, repeating the "═" symbol
    std::string h_line = "";
    for (int i = 0; i < m_width; ++i) {
        h_line += "═";
    }

    // --- 2. Add the Top Border ---
    map_output += "  ╔" + h_line + "╗\n";

    // --- 3. Add the Map Rows with Vertical Borders ---
    for (int y = 0; y < m_height; ++y) {
        map_output += "  ║"; // Left vertical border
        
        for (int x = 0; x < m_width; ++x) {
            // Append the tile's character (or string)
            map_output += m_grid[y][x].getMapDisplayChar();
        }
        
        map_output += "║\n"; // Right vertical border and newline
    }

    // --- 4. Add the Bottom Border ---
    map_output += "  ╚" + h_line + "╝\n";

    // --- 5. Print the complete map ---
    std::cout << map_output;
}

Tile* Map::getTileAt(int x, int y) {
    if (x >= 0 && x < m_height && y >= 0 && y < m_width) {
        return &m_grid[x][y];
    }
    return nullptr; // Return null if coordinates are out of bounds
}

int Map::getWidth() const {
    return m_width;
}

int Map::getHeight() const {
    return m_height;
}