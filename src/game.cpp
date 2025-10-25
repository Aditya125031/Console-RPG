#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include<cmath>
#include <limits>
#include <cstdlib> // Required for std::system
#include <conio.h> // Required for _getch()
#include <thread>  // Add this header at the top of your file
#include <chrono>  // Add this header at the top of your file
#include <windows.h>

using namespace std;

// Your project headers
#include "../include/game.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/map.h"
#include "../include/character.h"
#include "../include/tile.h"
#include "../include/items.h"
#include "../include/goblin.h"
#include "../include/colors.h" // You included this, which is great!

// --- CONSTANTS ---
// You should move this to a constants.h file later
const unsigned int MAX_LOG_LINES = 10;


// --- Helper Functions ---

// Clears the console screen
void clear_screen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void getTDimensions(int& width, int& height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

// Handles a single combat encounter
// CORRECTED: Changed the parameter name from 'enemy' to 'target' for clarity
void run_combat(Player& hero, Enemy& target) {
    clear_screen(); // Clear the map screen for combat
    std::cout << "\n--- A wild " << target.get_name() << " appears! ---\n";

    while (hero.isAlive() && target.isAlive()) {
        std::cout << "\n" << hero.get_name() << " HP: " << hero.get_health() << " | "
                  << target.get_name() << " HP: " << target.get_health() << "\n";
        std::cout << "Choose your action:\n";
        std::cout << "1. Attack\n";
        std::cout << "2. Special Move\n";
        std::cout << "Your choice: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            hero.attack(target);
        } else if (choice == 2) {
            hero.special_move(target);
        } else {
            std::cout << "Invalid choice, you hesitate!\n";
        }

        if (!target.isAlive()) break;

        std::cout << "\nThe " << target.get_name() << " attacks you!\n";
        target.attack(hero);
    }

    if (hero.isAlive()) {
        std::cout << "\nYou defeated the " << target.get_name() << "! Victory!\n";
        // Pause to let the player see the result before returning to map
        std::cout << "Press any key to continue...";
        _getch();
    } else {
        std::cout << "\nYou have been defeated. Game Over.\n";
    }
}

void Game::add_log_message(std::string message) {
    event_log.push_front(message);
    while (event_log.size() > MAX_LOG_LINES) {
        event_log.pop_back();
    }
}

void Game::display_dashboard(Player& player, Map& map) {
    clear_screen();
    int term_width = 0;
    int term_height = 0;
    getTDimensions(term_width, term_height);
    string text = "========================== The Defeated ==========================\n";
    int text_length = text.length();
    int left_padding = std::max(0, (term_width - text_length) / 2);
    std::string pad_str(left_padding, ' ');
    // Header
    std::cout << pad_str << "========================== The Defeated ==========================\n";
    std::cout << " Name: " << Color::FG_CYAN << player.get_name() << Color::RESET
              << " [" << player.get_type_string() << "]"
              << "\t HP: " << Color::FG_GREEN << player.get_health()
              << " / " << player.get_max_health() << Color::RESET << "\n";
    std::cout << "────────────────────────────────────────────────────────\n";

    // Middle Section (Minimap + Log)
    int minimap_height = 15;
    int minimap_width = 25;
    map.get_minimap_view(player, minimap_width, minimap_height, event_log);

    // Footer
    std::cout << "\n\n CONTROLS: (W/A/S/D) Move | (M) Full Map | (Q) Quit to Village\n";
    std::cout << " Your Location: (" << player.get_y() << ", " << player.get_x() << ")\n";
}

void Game::show_full_map(Map& map) {
    clear_screen();
    std::cout << "--- FULL MAP ---\n";
    map.render();
    std::cout << "\n(Press 'M' or 'Esc' to close)";
    
    while (true) {
        char input = _getch();
        if (input == 'm' || input == 27) { // 27 is ASCII for Escape
            break;
        }
    }
}

void Game::explore_forest(Player& player, Map& map) {
    add_log_message("You entered the forest.");
    
    // NOTE: In a real game, you would have a list of enemies loaded from the map
    Goblin goblin("Grug", 50, 8);
    
    bool inForest = true;
    while (inForest) {
        display_dashboard(player, map);
        char input = _getch(); // Get input without waiting for Enter

        switch(input) {
            case 'w':
                move_character(player, 0, -1, map);
                break;

            case 's':
                move_character(player, 0, 1, map);
                break;

            case 'a':
                move_character(player, -1, 0, map);
                break;

            case 'd':
                move_character(player, 1, 0, map);
                break;

            case 'm':
                show_full_map(map);
                break;

            case 'q':
                add_log_message("You return to the village.");
                inForest = false;
                break;
                
            default:
                add_log_message("Invalid key pressed.");
        }
    }
}

void Game::game_loop(Player& player) {
    Map stage1(player, 151, 39, "../data/map.txt");

    std::cout << "\n--- You find your way to a nearby village to rest. ---\n";
    bool isGameRunning = true;

    while (isGameRunning) {
        std::cout << "\n--- Village Menu ---\n";
        std::cout << "1. Show My Stats\n";
        std::cout << "2. Venture into the forest\n";
        std::cout << "3. Exit Game\n";
        std::cout << "Choice: ";

        int menuChoice;
        std::cin >> menuChoice;

        switch (menuChoice) {
            case 1:
                player.show_details();
                break;
            case 2:
                explore_forest(player, stage1);
                if (!player.isAlive()) {
                    isGameRunning = false;
                }
                break;
            case 3:
                std::cout << "Thanks for playing!\n";
                isGameRunning = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    }
}

void Game::move_character(Character& entity, int x, int y, Map& map){
    int newx=entity.get_x()+y;
    int newy=entity.get_y()+x;
    if(map.getTileAt(newx,newy)->getCharacter() != nullptr) {
        add_log_message("Combat Triggered!");
        // int k=startCombat(myPlayer, map->getTileAt(newy,newx)->getCharacter());
        // if(k==0){
        //     //end game
        //     cout << "You have been defeated!"<<endl;
        // }
        // else if(k==1){
        //     cout<<"You defeated the enemy!"<<endl;
        //     map->getTileAt(newy,newx)->setisWalkable();
        //     map->getTileAt(coord_y,coord_x)->setCharacter(nullptr);
        //     map->getTileAt(newy,newx)->setCharacter(this);
        //     coord_x=newx;
        //     coord_y=newy;
        // }
        // else if(k==2){
        //     cout<<"You fled the battle!"<<endl;
        // }
        return;
    } 
    // else if(map->getTileAt(newy,newx)->getItem()) {
    //     cout << "You found an item!" << endl;
    //     int k=pickItem(myPlayer, map->getTileAt(newy,newx)->getItem());
    //     if(k==1){
    //         cout << "You picked up the item!" << endl;
    //         map->getTileAt(newy,newx)->setItem(nullptr);
    //         map->getTileAt(coord_y,coord_x)->setCharacter(nullptr);
    //         map->getTileAt(newy,newx)->setCharacter(this);
    //         coord_x=newx;
    //         coord_y=newy;
    //     } else {
    //         cout << "You didn't pick up the item!" << endl;
    //     }
    //     return;
    // } 
    else if(map.getTileAt(newx,newy)->getBounds()) {
        add_log_message("Do not venture outside the forest!");
        return;
    } 
    else if(map.getTileAt(newx,newy)->getIsWalkable()) {
        map.getTileAt(entity.get_x(),entity.get_y())->setCharacter(nullptr);
        map.getTileAt(newx,newy)->setCharacter(&entity);
        map.getTileAt(newx,newy)->setMiniMapDisplayChar(Color::FG_YELLOW + "♞" + Color::RESET);
        map.getTileAt(entity.get_x(),entity.get_y())->setMiniMapDisplayChar(".");
        map.getTileAt(newx,newy)->setMapDisplayChar(Color::FG_YELLOW + "♞" + Color::RESET);
        map.getTileAt(entity.get_x(),entity.get_y())->setMapDisplayChar(".");
        entity.set_x(newx);
        entity.set_y(newy);
        add_log_message("You moved to (" + to_string(entity.get_y()) + ", " + to_string(entity.get_x()) + ").");
        return;
    }
    else {
        add_log_message("You can't move there!");
        return;
    }
    add_log_message("Achievement Unlocked! The Void!");

}