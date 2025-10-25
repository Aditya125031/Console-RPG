#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include<cmath>
#include <limits>
#include <chrono>
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
void display_combat_ui(const Player& player, const Enemy& enemy) {
    clear_screen();
    auto now = std::chrono::steady_clock::now();
    float normal_cd = std::chrono::duration<float>(player.getNormalAttackReady() - now).count();
    float special_cd = std::chrono::duration<float>(player.getSpecialAttackReady() - now).count();
    if (normal_cd < 0) normal_cd = 0.0f;
    if (special_cd < 0) special_cd = 0.0f;

    std::cout << "===================== BATTLE =====================\n";
    
    std::cout << " " << player.get_name() << " [" << player.get_type_string() << "]\n";
    std::cout << " " << Color::GREEN << "HP:  " << player.get_health() << " / " << player.get_max_health() << Color::RESET << "\n";
    std::cout << " " << Color::BLUE << "MANA:" << player.getMana() << " / " << player.getMaxMana() << Color::RESET << "\n";

    std::cout << "\n " << Color::RED << enemy.get_name() << Color::RESET << "\n";
    std::cout << " " << Color::RED << "HP:  " << enemy.get_health() << " / " << enemy.get_max_health() << Color::RESET << "\n";
    
    std::cout << "===================== ACTIONS ====================\n";
    
    std::cout << " (1) Normal Attack (10 Mana) | Ready in: ";
    if (normal_cd == 0.0f) std::cout << Color::GREEN << "READY" << Color::RESET << "\n";
    else std::cout << std::fixed << std::setprecision(1) << normal_cd << "s\n";
    
    std::cout << " (2) Special Move  (25 Mana) | Ready in: ";
    if (special_cd == 0.0f) std::cout << Color::GREEN << "READY" << Color::RESET << "\n";
    else std::cout << std::fixed << std::setprecision(1) << special_cd << "s\n";

    std::cout << "==================================================\n";
}
void getTDimensions(int& width, int& height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

// Handles a single combat encounter
// CORRECTED: Changed the parameter name from 'enemy' to 'target' for clarity
void run_combat(Player& player, Enemy& enemy) {
    std::cout << "\n--- You've encountered a " << enemy.get_name() << "! ---\n";
    enemy.triggerDialogue();
    std::cout << "(Press any key to begin...)";
    _getch();

    // Initialize attack times to be ready now
    player.set_normal_attack_cooldown(0);
    player.set_special_attack_cooldown(0);
    enemy.setNormalAttackCooldown(0); // Enemy can attack immediately

    while (player.isAlive() && enemy.isAlive()) {
        auto current_time = std::chrono::steady_clock::now();

        // 1. --- Handle Player Input ---
        if (_kbhit()) { // Check if a key is pressed
            char input = _getch(); // Get the key without waiting

            if (input == '1') { // --- Normal Attack ---
                if (current_time >= player.getNormalAttackReady()) {
                    if (player.get_mana() >= 10) {
                        player.attack(enemy);
                        player.use_mana(10);
                        player.set_normal_attack_cooldown(1.0f); // 1 second cooldown
                    } else {
                        // Not enough mana (log this later)
                    }
                } else {
                    // Not ready (log this later)
                }
            } else if (input == '2') { // --- Special Attack ---
                if (current_time >= player.getSpecialAttackReady()) {
                    if (player.get_mana() >= 25) {
                        player.special_move(enemy);
                        player.use_mana(25);
                        player.set_special_attack_cooldown(2.5f); // 2.5 second cooldown
                    } else {
                        // Not enough mana
                    }
                } else {
                    // Not ready
                }
            }
        }
        
        // 2. --- Handle Enemy AI ---
        if (current_time >= enemy.getNormalAttackReady()) {
            // Simple AI: 30% chance to use special, 70% normal
            if (rand() % 10 < 3) {
                // enemy.specialAbility(player); // (You'll need to add cooldowns for this too)
                enemy.attack(player); // For now, just a normal attack
            } else {
                enemy.attack(player);
            }
            enemy.setNormalAttackCooldown(2.0f); // Enemy attacks every 2 seconds
        }

        // 3. --- Handle Regenerat
        player.update_mana_regen(current_time);

        // 4. --- Draw the UI ---
        display_combat_ui(player, enemy);
    }

    // 6. --- End of Combat ---
    clear_screen();
    std::cout << "--------------------------------\n";
    if (player.isAlive()) {
        std::cout << "You defeated the " << enemy.get_name() << "! Victory!\n";
    } else {
        std::cout << "You have been defeated. Game Over.\n";
    }
    std::cout << "(Press any key to continue...)\n";
    _getch(); // Wait for user
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
        //cout << player.get_x() << " " << player.get_y() << endl;

        char input = _getch(); // Get input without waiting for Enter

        switch(input) {
            case 'w':
                add_log_message(player.move(0, -1,map));
                break;

            case 's':
                add_log_message(player.move(0, 1, map));
                break;

            case 'a':
                add_log_message(player.move(-1, 0, map));
                break;

            case 'd':
                add_log_message(player.move(1, 0, map));
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