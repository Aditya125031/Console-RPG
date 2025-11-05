#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "../extern/pdcurses/curses.h" 

using namespace std;

#include "../include/game.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/map.h"
#include "../include/character.h"
#include "../include/items.h"
#include "../include/goblin.h"

const unsigned int MAX_LOG_LINES = 10;

int run_combat(Player& hero, Enemy& target) {
    clear();
    int row = 0;
    
    mvprintw(row++, 0, "\n--- A wild %s appears! ---", target.get_name().c_str());

    while (hero.isAlive() && target.isAlive()) {
        row++;
        mvprintw(row++, 0, "%s HP: %d | %s HP: %d",
                 hero.get_name().c_str(), hero.get_health(),
                 target.get_name().c_str(), target.get_health());
        
        mvprintw(row++, 0, "Choose your action:");
        mvprintw(row++, 0, "1. Attack");
        mvprintw(row++, 0, "2. Special Move");
        mvprintw(row++, 0, "3. Flee");
        mvprintw(row, 0, "Your choice: ");
        refresh();

        int choice;
        echo();
        scanw("%d", &choice);
        noecho();
        row++;

        if (choice == 1) {
            hero.attack(target);
        } else if (choice == 2) {
            hero.special_move(target);
        }
        else if(choice==3) {
            return 2;
        }
        else {
            mvprintw(row++, 0, "Invalid choice, you hesitate!");
            refresh();
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        if (!target.isAlive()) break;

        mvprintw(row++, 0, "\nThe %s attacks you!", target.get_name().c_str());
        target.attack(hero);
        refresh();
        this_thread::sleep_for(chrono::milliseconds(500)); 
        
        clear();
        row = 0;
    }
    
    clear();
    if (hero.isAlive()) {
        mvprintw(0, 0, "You defeated the %s! Press any key to continue...", target.get_name().c_str());
        refresh();
        getch();
        return 1;
    } else {
        mvprintw(0, 0, "You have been defeated! Press any key to continue...", hero.get_name().c_str());
        refresh();
        getch();
        return 0;
    }
}

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
    getmaxyx(stdscr, term_height, term_width);
    
    string title = "========================== The Defeated ==========================";
    int title_length = title.length();
    int left_padding = std::max(0, (term_width - title_length) / 2);
    
    int row = 0;

    mvprintw(row++, left_padding, "%s", title.c_str());

    mvprintw(row, 0, " Name: ");
    
    attron(COLOR_PAIR(1));
    printw("%s", player.get_name().c_str());
    attroff(COLOR_PAIR(1));

    printw(" [%s]\t HP: ", player.get_type_string().c_str());
    
    attron(COLOR_PAIR(2));
    printw("%d / %d", player.get_health(), player.get_max_health());
    attroff(COLOR_PAIR(2));
    
    row++;

    mvprintw(row++, 0, "────────────────────────────────────────────────────────");

    int minimap_height = 15;
    int minimap_width = 25;
    map.get_minimap_view(player, minimap_width, minimap_height, event_log);

    mvprintw(term_height - 3, 0, " CONTROLS: (W/A/S/D) Move | (M) Full Map | (Q) Quit to Village");
    mvprintw(term_height - 2, 0, " Your Location: (%d, %d)", player.get_y(), player.get_x());
    
    refresh();
}

void Game::show_full_map(Map& map) {
    bool onFullMap = true;

    while (onFullMap) {
        map.render(); 
        if (is_termresized()) {
            resize_term(0, 0);
            clear();
            refresh();
            continue; 
        }
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

void Game::explore_forest(Player& player, Map& map, vector<bool>& quest) {
    add_log_message("You entered the forest.");
    bool inForest = true;
    while (inForest) {
        display_dashboard(player, map);
        if (is_termresized()) {
            resize_term(0, 0);
            clear();
            refresh();
            continue;
        }
        int input = getch(); 

        switch(input) {
            case 'w':
            case 'W':
                move_character(player, 0, -1, map, quest);
                break;

            case 's':
            case 'S':
                move_character(player, 0, 1, map, quest);
                break;

            case 'a':
            case 'A':
                move_character(player, -1, 0, map, quest);
                break;

            case 'd':
            case 'D':
                move_character(player, 1, 0, map, quest);
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
        flushinp();
    }
}

void Game::game_loop(Player& player) {
    clear();
    vector<bool> quest(5,false);
    Map stage1(player, quest, 153, 37, "../data/map.txt");
    printw("\n--- You find your way to a nearby village to rest. ---\n");
    bool isGameRunning = true;
    refresh();

    while (isGameRunning) {
        clear(); 
        int row = 0;
        
        mvprintw(row++, 0, "\n--- Village Menu ---");
        mvprintw(row++, 0, "1. Show My Stats");
        mvprintw(row++, 0, "2. Venture into the forest");
        mvprintw(row++, 0, "3. Exit Game");
        mvprintw(row, 0, "Choice: ");
        refresh();
        if (is_termresized()) {
            resize_term(0, 0);
            clear();
            refresh();
            continue; 
        }
        int menuChoice = getch();
        switch (menuChoice) {
            case '1':
                clear();
                player.show_details(); 
                mvprintw(getmaxy(stdscr) - 2, 0, "Press any key to return...");
                refresh();
                getch();
                break;
            case '2':
                explore_forest(player, stage1, quest);
                if (!player.isAlive()) {
                    isGameRunning = false;
                }
                break;
            case '3':
                mvprintw(row + 1, 0, "Thanks for playing!");
                refresh();
                this_thread::sleep_for(chrono::seconds(1));
                flushinp();
                isGameRunning = false;
                break;
            default:
                mvprintw(row + 1, 0, "Invalid choice. Please try again.");
                refresh();
                this_thread::sleep_for(chrono::seconds(1));
                flushinp();
                break;
        }
    }
}

void Game::move_character(Character& entity, int x, int y, Map& map, vector<bool>& quest) {
    int newx=entity.get_x()+y;
    int newy=entity.get_y()+x;
    if(map.getTileAt(newx,newy)->getCharacter() != nullptr) {
        if(!map.getTileAt(newx,newy)->getQuestStatus(quest)){
            add_log_message("You are not not powerful enough!");
            add_log_message("Meet Hattori at (X,X)");
            return;
        }
        Player& player = static_cast<Player&>(entity);
        Character* target_ptr = map.getTileAt(newx,newy)->getCharacter();
        Enemy& target = static_cast<Enemy&>(*target_ptr);
        add_log_message("Combat Triggered!");
        int k = run_combat(player, target);
        if(k==0){
            clear();
            mvprintw(0, 0, "You have been defeated! Press any key to exit.");
            refresh();
            getch();
        }
        else if(k==1){
            add_log_message("You defeated the enemy");
            if(map.getTileAt(newx,newy)->get_doQuest()!=-1){
                quest[map.getTileAt(newx,newy)->get_doQuest()]=true;
            }
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