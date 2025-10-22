#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include<cmath>
#include <limits>
#include <cstdlib> // Required for std::system
#include <conio.h> // Required for _getch()

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

    // Header
    std::cout << "========================== C++ RPG ==========================\n";
    std::cout << " Name: " << Color::CYAN << player.get_name() << Color::RESET
              << " [" << player.get_type_string() << "]"
              << "\t HP: " << Color::GREEN << player.get_health()
              << " / " << player.get_max_health() << Color::RESET << "\n";
    std::cout << "------------------------------------------------------------\n";

    // Middle Section (Minimap + Log)
    int minimap_height = 11;
    int minimap_width = 20;
    std::vector<std::string> minimap = map.get_minimap_view(player, minimap_width, minimap_height);

    for (int i = 0; i < minimap_height; ++i) {
        // Print the minimap line
        if (i < minimap.size()) {
            std::cout << minimap[i];
        } else {
            std::cout << std::string(minimap_width * 2, ' ');
        }
        
        // Print the event log line
        std::cout << "   | ";
        if (i < event_log.size()) {
            std::cout << Color::GRAY << event_log[i] << Color::RESET;
        }
        std::cout << "\n";
    }

    // Footer
    std::cout << "------------------------------------------------------------\n";
    std::cout << " CONTROLS: (W/A/S/D) Move | (M) Full Map | (Q) Quit to Village\n";
    std::cout << " Your Location: (" << player.get_x() << ", " << player.get_y() << ")\n";
}

void Game::show_full_map(Map& map) {
    clear_screen();
    std::cout << "--- FULL MAP --- (Press 'M' or 'Esc' to close)\n";
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
                if (player.get_y() > 0) { // TODO: Check for walls
                    player.move(0, -1);
                    add_log_message("You moved north.");
                } else {
                    add_log_message("You can't go further north!");
                }
                break;
            case 's':
                if (player.get_y() < map.getHeight() - 1) { // Boundary check
                    player.move(0, 1);
                    add_log_message("You moved south.");
                } else {
                    add_log_message("You can't go further south!");
                }
                break;
            case 'a':
                if (player.get_x() > 0) {
                    player.move(-1, 0);
                    add_log_message("You moved west.");
                } else {
                    add_log_message("You can't go further west!");
                }
                break;
            case 'd':
                if (player.get_x() < map.getWidth() - 1) { // Boundary check
                    player.move(1, 0);
                    add_log_message("You moved east.");
                } else {
                    add_log_message("You can't go further east!");
                }
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
    Map stage1(156, 40, "data/map.txt");

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