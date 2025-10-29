#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>
#include <thread>
#include <chrono>
#include <cstdlib> // Required for std::system
//#include <windows.h> // Not needed with curses
#include "../extern/pdcurses/curses.h" // Include the curses header
#include "../include/tile.h"
#include "../include/map.h"
#include "../include/character.h"
#include "../include/player.h"
#include "../include/items.h"
#include "../include/game.h"
#include "../include/enemy.h"

// Note: I'm assuming 'PlayerType' is an enum defined in one of your headers.

Player create_player() {
    clear();
    mvprintw(0, 0, "Welcome to Text RPG!");

    // --- NAME INPUT ---
    mvprintw(1, 0, "Please enter your hero's name: ");
    
    // ⭐️ FIX: Temporarily turn on echo() to see typing
    echo(); 
    refresh();

    std::string playerName;
    char nameBuffer[50];
    getnstr(nameBuffer, sizeof(nameBuffer) - 1);
    playerName = nameBuffer;

    mvprintw(3, 0, "Choose your class:");
    mvprintw(4, 0, "1. Swordsman");
    mvprintw(5, 0, "2. Archer");
    mvprintw(6, 0, "3. Mage:");
    mvprintw(8, 0, "Enter choice (1-3): ");
    
    // ⭐️ FIX: Temporarily enable line buffering for scanw
    nocbreak();
    refresh();

    int choice;
    scanw("%d", &choice);

    // ⭐️ FIX: Restore game state
    noecho(); 
    cbreak(); 

    PlayerType playerType;

    switch (choice) {
        case 1: playerType = PlayerType::Swordsman; break;
        case 2: playerType = PlayerType::Archer; break;
        case 3: playerType = PlayerType::Mage; break;
        default:
            mvprintw(10, 0, "Invalid choice. Defaulting to Swordsman...");
            refresh();
            playerType = PlayerType::Swordsman;
            this_thread::sleep_for(chrono::seconds(1));
            flushinp();
            break;
    }

    refresh();
    return Player(playerName, playerType);
}

// This function should be called once, before starting the game loop
void setup_curses() {
    initscr();
    
    // --- Standard Game Terminal Setup ---
    cbreak();   // ✅ CORRECT: Disables line buffering (reacts instantly)
    noecho();   // ✅ CORRECT: Prevents input keys from being printed
    keypad(stdscr, TRUE); // Enables detection of special keys
    
    // --- Color Setup ---
    if (has_colors()) {
        start_color(); // Starts the color system

        // Initialize the color pairs using your defined constants
        init_pair(1, COLOR_CYAN, COLOR_BLACK); 
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_BLUE, COLOR_BLACK); 
        init_pair(4, COLOR_RED, COLOR_BLACK);
        init_pair(5, COLOR_YELLOW, COLOR_BLACK); 
        init_pair(6, COLOR_WHITE, COLOR_BLACK); 
        init_pair(7, COLOR_MAGENTA, COLOR_BLACK);  
    }
    
    // The timeout below is optional, but often useful for game loops
    // sets the waiting period for getch() in milliseconds.
    //timeout(100); 
}

//---

int main()
{
    setup_curses();
    Game world;
    Player hero = create_player();
    world.game_loop(hero);
    endwin();
    
    return 0;
}