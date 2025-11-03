#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <thread>
#include <chrono>

// REMOVE: #include <iostream>
// REMOVE: #include <windows.h>

// ADD PDCURSES HEADER
#include "../extern/pdcurses/curses.h" // Assuming this is your path

using namespace std;

// Your project headers (kept for completeness)
#include "../include/game.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/map.h"
#include "../include/character.h"
#include "../include/tile.h"
#include "../include/items.h"
#include "../include/goblin.h"
#include "../include/colors.h"

// --- CONSTANTS ---
const unsigned int MAX_LOG_LINES = 10;

// Handles a single combat encounter
int run_combat(Player& hero, Enemy& target) {
    clear();
    int row = 0;
    
    // ⭐️ REPLACED: std::cout with mvprintw
    mvprintw(row++, 0, "\n--- A wild %s appears! ---", target.get_name().c_str());

    while (hero.isAlive() && target.isAlive()) {
        row++;
        // ⭐️ REPLACED: std::cout with mvprintw
        mvprintw(row++, 0, "%s HP: %d | %s HP: %d",
                 hero.get_name().c_str(), hero.get_health(),
                 target.get_name().c_str(), target.get_health());
        
        mvprintw(row++, 0, "Choose your action:");
        mvprintw(row++, 0, "1. Attack");
        mvprintw(row++, 0, "2. Special Move");
        mvprintw(row++, 0, "3. Flee");
        mvprintw(row, 0, "Your choice: ");
        refresh(); // Refresh to show the menu

        int choice;
        // ⭐️ REPLACED: std::cin >> choice with scanw
        scanw("%d", &choice);
        row++; // Move past the input line

        if (choice == 1) {
            hero.attack(target);
        } else if (choice == 2) {
            hero.special_move(target);
        }
        else if(choice==3) {
            return 2; // Flee
        }
        else {
            // ⭐️ REPLACED: std::cout with mvprintw
            mvprintw(row++, 0, "Invalid choice, you hesitate!");
            refresh();
            this_thread::sleep_for(chrono::milliseconds(500)); // Pause briefly
        }

        if (!target.isAlive()) break;

        // ⭐️ REPLACED: std::cout with mvprintw
        mvprintw(row++, 0, "\nThe %s attacks you!", target.get_name().c_str());
        target.attack(hero);
        refresh();
        this_thread::sleep_for(chrono::milliseconds(500)); // Pause briefly
        
        // Clear old combat text for the next turn
        clear();
        row = 0;
    }
    
    // Final result message
    clear();
    if (hero.isAlive()) {
        mvprintw(0, 0, "You defeated the %s! Press any key to continue...", target.get_name().c_str());
        refresh();
        getch(); // ⭐️ getch() instead of _getch()
        return 1; // Win
    } else {
        mvprintw(0, 0, "You have been defeated! Press any key to continue...", hero.get_name().c_str());
        refresh();
        getch(); // ⭐️ getch() instead of _getch()
        return 0; // Loss
    }
}

// ... (add_log_message remains the same)

void Game::add_log_message(std::string message) {
    event_log.push_front(message);
    while (event_log.size() > MAX_LOG_LINES) {
        event_log.pop_back();
    }
}


void Game::display_dashboard(Player& player, Map& map) {
    clear(); 
    int term_width = 0;
    int term_height = 0;
    getmaxyx(stdscr, term_height, term_width); // Curses get dimensions
    
    string title = "========================== The Defeated ==========================";
    int title_length = title.length();
    int left_padding = std::max(0, (term_width - title_length) / 2);
    
    int row = 0;

    // Header
    mvprintw(row++, left_padding, "%s", title.c_str());

    // 1. Start printing the static text
    mvprintw(row, 0, " Name: ");
    
    // 2. Print Name in Cyan
    attron(COLOR_PAIR(1));
    printw("%s", player.get_name().c_str());
    attroff(COLOR_PAIR(1));

    // 3. Print the text that follows
    printw(" [%s]\t HP: ", player.get_type_string().c_str());
    
    // 4. Print HP in Green
    attron(COLOR_PAIR(2));
    printw("%d / %d", player.get_health(), player.get_max_health());
    attroff(COLOR_PAIR(2));
    
    // 5. Move to the next line
    row++;

    mvprintw(row++, 0, "────────────────────────────────────────────────────────");

    // Middle Section (Minimap + Log)
    int minimap_height = 15;
    int minimap_width = 25;
    // NOTE: map.get_minimap_view must be updated to use curses (mvprintw) inside it!
    map.get_minimap_view(player, minimap_width, minimap_height, event_log);

    // Footer
    mvprintw(term_height - 3, 0, " CONTROLS: (W/A/S/D) Move | (M) Full Map | (Q) Quit to Village");
    mvprintw(term_height - 2, 0, " Your Location: (%d, %d)", player.get_y(), player.get_x());
    
    refresh(); // Show everything
}

void Game::show_full_map(Map& map) {
    bool onFullMap = true;

    while (onFullMap) {
        map.render(); 
        // ⭐️ --- ADD THIS BLOCK --- ⭐️
        if (is_termresized()) {
            resize_term(0, 0);
            clear();
            refresh();
            continue; 
        }
        // ⭐️ ------------------------ ⭐️
        int input = getch(); 

        switch (input) {
            case 'm': 
            case 'q': 
                onFullMap = false;
                break;

            default:
                break;
        }
        refresh();
    }

    clear(); 
}

void Game::explore_forest(Player& player, Map& map) {
    add_log_message("You entered the forest.");
    bool inForest = true;
    while (inForest) {
        display_dashboard(player, map);
        // ⭐️ --- ADD THIS BLOCK --- ⭐️
        if (is_termresized()) {
            resize_term(0, 0);
            clear();
            refresh();
            continue; // Force the loop to restart and redraw
        }
        // ⭐️ ------------------------ ⭐️
        int input = getch(); 

        switch(input) {
            case 'w':
            case 'W':
                move_character(player, 0, -1, map);
                break;

            case 's':
            case 'S':
                move_character(player, 0, 1, map);
                break;

            case 'a':
            case 'A':
                move_character(player, -1, 0, map);
                break;

            case 'd':
            case 'D':
                move_character(player, 1, 0, map);
                break;

            case 'm':
            case 'M':
                show_full_map(map);
                break;

            case 'q':
            case 'Q':
                add_log_message("You return to the village.");
                inForest = false;
                break;
                
            default:
                add_log_message("Invalid key pressed.");
        }
        this_thread::sleep_for(chrono::milliseconds(150));
        flushinp();
    }
}

void Game::game_loop(Player& player) {
    Map stage1(player, 153, 37, "../data/map.txt");
    // ⭐️ REPLACED: std::cout with printw
    printw("\n--- You find your way to a nearby village to rest. ---\n");
    bool isGameRunning = true;
    refresh();

    while (isGameRunning) {
        clear(); 
        int row = 0;
        
        // ⭐️ REPLACED: std::cout with mvprintw
        mvprintw(row++, 0, "\n--- Village Menu ---");
        mvprintw(row++, 0, "1. Show My Stats");
        mvprintw(row++, 0, "2. Venture into the forest");
        mvprintw(row++, 0, "3. Exit Game");
        mvprintw(row, 0, "Choice: ");
        refresh();
        // ⭐️ --- ADD THIS BLOCK --- ⭐️
        if (is_termresized()) {
            resize_term(0, 0);
            clear();
            refresh();
            continue; // Force the loop to restart and redraw
        }
        int menuChoice = getch();
        // ⭐️ ------------------------ ⭐️
        switch (menuChoice) {
            case '1':
                clear();
                player.show_details(); 
                mvprintw(getmaxy(stdscr) - 2, 0, "Press any key to return...");
                refresh();
                getch();
                break;
            case '2':
                explore_forest(player, stage1);
                if (!player.isAlive()) {
                    isGameRunning = false;
                }
                break;
            case '3':
                // ⭐️ REPLACED: std::cout with mvprintw
                mvprintw(row + 1, 0, "Thanks for playing!");
                refresh();
                this_thread::sleep_for(chrono::seconds(1));
                flushinp();
                isGameRunning = false;
                break;
            default:
                // ⭐️ REPLACED: std::cout with mvprintw, and removed cin error handling
                mvprintw(row + 1, 0, "Invalid choice. Please try again.");
                refresh();
                this_thread::sleep_for(chrono::seconds(1));
                flushinp();
                break;
        }
    }
}

void Game::move_character(Character& entity, int x, int y, Map& map){
    int newx=entity.get_x()+y;
    int newy=entity.get_y()+x;
    if(map.getTileAt(newx,newy)->getCharacter() != nullptr) {
        Player& player = static_cast<Player&>(entity);
        // NOTE: You need to be careful with the lifetime of the Goblin here.
        // If it's stored on the map, it should be a pointer/unique_ptr.
        Character* target_ptr = map.getTileAt(newx,newy)->getCharacter();
        Enemy& target = static_cast<Enemy&>(*target_ptr);
        add_log_message("Combat Triggered!");
        int k = run_combat(player, target);
        if(k==0){
            // ⭐️ REPLACED: std::cout with mvprintw
            clear();
            mvprintw(0, 0, "You have been defeated! Press any key to exit.");
            refresh();
            getch();
        }
        else if(k==1){
            add_log_message("You defeated the enemy");
            map.getTileAt(newx,newy)->setIsWalkable(true);
            map.getTileAt(entity.get_x(), entity.get_y())->setCharacter(nullptr);
            map.getTileAt(entity.get_x(), entity.get_y())->set_map_color_pair(6);
            map.getTileAt(entity.get_x(), entity.get_y())->set_mini_map_color_pair(6);
            map.getTileAt(newx,newy)->setCharacter(&entity);
            map.getTileAt(newx,newy)->setMiniMapDisplayChar("♞");
            map.getTileAt(entity.get_x(),entity.get_y())->setMiniMapDisplayChar(".");
            map.getTileAt(newx,newy)->setMapDisplayChar("♞");
            map.getTileAt(entity.get_x(),entity.get_y())->setMapDisplayChar(".");
            map.getTileAt(newx,newy)->set_map_color_pair(5);
            map.getTileAt(newx,newy)->set_mini_map_color_pair(5);
            entity.set_x(newx);
            entity.set_y(newy);
        }
        else if(k==2){
            add_log_message("You fled the battle!");
        }
        return;
    } 
    // else if(map->getTileAt(newy,newx)->getItem()) { ... (Item handling)
    //     // Your commented code here would also need to use curses
    //     return;
    // } 
    else if(map.getTileAt(newx,newy)->getBounds()) {
        add_log_message("Do not venture outside the forest!");
        return;
    } 
    else if(map.getTileAt(newx,newy)->getIsWalkable()) {
        map.getTileAt(entity.get_x(),entity.get_y())->setCharacter(nullptr);
        map.getTileAt(newx,newy)->setCharacter(&entity);
        map.getTileAt(entity.get_x(), entity.get_y())->set_map_color_pair(6);
        map.getTileAt(entity.get_x(), entity.get_y())->set_mini_map_color_pair(6);
        map.getTileAt(newx,newy)->setMiniMapDisplayChar("♞");
        map.getTileAt(entity.get_x(),entity.get_y())->setMiniMapDisplayChar(".");
        map.getTileAt(newx,newy)->setMapDisplayChar("♞");
        map.getTileAt(entity.get_x(),entity.get_y())->setMapDisplayChar(".");
        map.getTileAt(newx,newy)->set_map_color_pair(5);
        map.getTileAt(newx,newy)->set_mini_map_color_pair(5);
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