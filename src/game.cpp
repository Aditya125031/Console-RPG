#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <sstream>
#include "../extern/pdcurses/curses.h"  
// REMOVE: #include <iostream>
// REMOVE: #include <windows.h>

using namespace std;

// Your project headers (kept for completeness)
#include "../include/combat.hpp"
#include"../include/inventory.hpp"
#include "../include/game.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/map.h"
#include "../include/character.h"
#include "../include/items.h"
#include "../include/goblin.h"
#include "../include/npc.h"

class AudioManager; // Forward declaration

const unsigned int MAX_LOG_LINES = 10;

void Game::show_dialogue_message(const std::string& message) {
    // This function just sets the message that will be drawn by the dashboard
    current_dialogue_message = message;
}

void Game::clear_dialogue_message() {
    current_dialogue_lines.clear();
}
void Game::display_dashboard(Player& player, Map& map) {
    clear(); 
    int term_width = 0;
    int term_height = 0;
    getmaxyx(stdscr, term_height, term_width);
    
    string title_prefix = "========================== THE ";
    string title_red = "DEFEATED";
    string title_suffix = " ==========================";
    
    int full_len = title_prefix.length() + title_red.length() + title_suffix.length();
    int left_padding = max(0, (term_width - full_len) / 2);
    int row = 0;

    // Print the title centered at row 0
    mvprintw(row, left_padding, "%s", title_prefix.c_str());
    attron(COLOR_PAIR(4) | A_BOLD); // Red for "DEFEATED"
    printw("%s", title_red.c_str());
    attroff(COLOR_PAIR(4) | A_BOLD);
    printw("%s", title_suffix.c_str());
    
    row += 2;

    mvprintw(row, 0, " Name:  ");
    attron(COLOR_PAIR(1));
    printw("%s", player.get_name().c_str());
    attroff(COLOR_PAIR(1));
    row++;
    mvprintw(row, 0, " Class: %s", player.get_type_string().c_str());
    row++;

    // 3. HP
    mvprintw(row, 0, " HP:    ");
    attron(COLOR_PAIR(2));
    printw("%d/%d", player.get_health(), player.get_max_health());
    attroff(COLOR_PAIR(2));
    row++;

    // 4. Mana
    mvprintw(row, 0, " Mana:  ");
    attron(COLOR_PAIR(1));
    // Note: Assuming get_mana() is current and you might need a get_max_mana() later
    printw("%d / %d", player.get_mana(), player.get_mana());
    attroff(COLOR_PAIR(1));
    row++;
    mvprintw(row++, 0, "────────────────────────────────────────────────────────");
    
    row+=12;
    mvprintw(row++, 2, "[ EQUIPMENT ]");
    if(player.inventory.equippedWeapon){
        mvprintw(row++, 2, " W: %s",player.inventory.equippedWeapon->get_item_name().c_str());
    } else {
        mvprintw(row++, 2, " W: (None)");
    }
    if(player.inventory.equippedArmor){
        mvprintw(row++, 2, " A: %s",player.inventory.equippedArmor->get_item_name().c_str());
    } else {
        mvprintw(row++, 2, " A: (None)");
    }

    int minimap_height = 17;
    int minimap_width = 29;
    map.get_minimap_view(player, minimap_width, minimap_height, event_log);
    mvprintw(term_height - 3, 0, " CONTROLS: (W/A/S/D) Move | (M) Full Map | (Q) Quit to Village | (I) Inventory ");
    mvprintw(term_height - 2, 0, " Your Location: (%d, %d)", player.get_y(), player.get_x());
    int dialogue_Y_line = 35;
    int i = 0;
    for (const std::string& line : current_dialogue_lines) 
    {
        // Center each line individually
        int start_x = (term_width - line.length()) / 2;
        if (start_x < 0) {
            start_x = 0; 
        }
        mvprintw(dialogue_Y_line + i, start_x, line.c_str());
        i++; 
    }
    refresh();
}
std::vector<std::string> Game::wrap_text(const std::string& text, int max_width) {
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string word;
    std::string current_line;

    while (ss >> word) {
        // Check if the new word fits on the current line
        if (current_line.empty() || current_line.length() + word.length() + 1 <= max_width) {
            if (!current_line.empty()) {
                current_line += " ";
            }
            current_line += word;
        } else {
            // Word doesn't fit, so push the current line and start a new one
            lines.push_back(current_line);
            current_line = word;
        }
    }
    // Add the last line
    if (!current_line.empty()) {
        lines.push_back(current_line);
    }
    return lines;
}
void Game::play_dialogue(const std::vector<std::string>& lines, Player& player, Map& map)
{
    
    int term_width, term_height;
    getmaxyx(stdscr, term_height, term_width);

    int dialogue_width = term_width - 10;
    if (dialogue_width < 20) dialogue_width = 20; 
    std::vector<std::string> all_wrapped_lines;
    for (const std::string& original_line : lines) {
        std::vector<std::string> wrapped = wrap_text(original_line, dialogue_width);
        all_wrapped_lines.insert(all_wrapped_lines.end(), wrapped.begin(), wrapped.end());
    }

    for (const std::string& line : all_wrapped_lines) {
        current_dialogue_lines.push_back(line); 
        display_dashboard(player, map); 
        getch(); 
        // --- END OF NEW LOGIC ---
    }
}


void Game::add_log_message(std::string message) {
    event_log.push_front(message);
    while (event_log.size() > MAX_LOG_LINES) {
        event_log.pop_back();
    }
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
void Game:: runItemActionMenu(DisplayItem selectedItem, Player& player, Game& world)
{
    clear();
    printw("Selected: %s\n Item Description: %s", selectedItem.displayName.c_str(),selectedItem.description.c_str());
    printw("------------------\n");
    
    // "Use" text changes based on item type
    if (selectedItem.type == "WEAPON" || selectedItem.type == "ARMOR") {
        printw("(e) Equip / Swap\n");
    } else if (selectedItem.type == "POTION") {
        printw("(u) Use\n");
    }
    
    printw("(d) Drop\n");
    printw("(c) Cancel\n");
    refresh();

    char ch = getch();
    switch (ch)
    {
        case 'e': // Equip/Swap
        case 'u': // Use
            if (selectedItem.type == "WEAPON") {
                player.inventory.swapWeapon(player, world);
            } else if (selectedItem.type == "ARMOR") {
                player.inventory.swapArmor(player, world);
            } else if (selectedItem.type == "POTION") {
                // We use the itemID, which holds the potion's name
                player.inventory.usePotion(selectedItem.itemID, player, world);
            }
            break;

        case 'd': // Drop
            if (selectedItem.itemID == "EQUIPPED_WEAPON") {
                player.inventory.dropEquippedWeapon(world, player);
            } else if (selectedItem.itemID == "INVENTORY_WEAPON") {
                player.inventory.dropInventoryWeapon(world);
            } else if (selectedItem.itemID == "EQUIPPED_ARMOR") {
                player.inventory.dropEquippedArmor(world, player);
            } else if (selectedItem.itemID == "INVENTORY_ARMOR") {
                player.inventory.dropInventoryArmor(world);
            } else if (selectedItem.type == "POTION") {
                // We use the itemID, which holds the potion's name
                player.inventory.dropPotion(selectedItem.itemID, world);
            }
            break;
            
        case 'c': // Cancel
        default:
            break;
    }
}
void Game:: runInventoryMenu(Player& player, Game& world)
{
    bool inInventory = true;
    while(inInventory)
    {
        clear();
        printw("--- INVENTORY --- (Select # or 'q' to quit)\n\n");

        // This vector maps a menu index (1, 2, 3...) to an item
        vector<DisplayItem> itemMap=buildPlayerItemList(player);
        // --- Display the list ---
        if (itemMap.empty()) {
            printw(" (Empty)\n");
        } else {
            for (int i = 0; i < itemMap.size(); ++i) {
                printw(" %d: %s\n", i + 1, itemMap[i].displayName.c_str());
            }
        }
        
        printw("\nSelect item #: ");
        refresh();

        // --- Get user input ---
        char ch = getch();

        if (ch == 'q') {
            inInventory = false; // Exit loop
            continue;
        }

        // Convert char '1' to int 1
        int choice = ch - '0';

        // Check if choice is valid
        if (choice > 0 && choice <= itemMap.size())
        {
            // Valid item selected!
            DisplayItem selectedItem = itemMap[choice - 1]; // -1 for 0-indexing
            runItemActionMenu(selectedItem, player, world);
        }
        else {
            // Invalid key
            printw("\nInvalid selection. Press any key.");
            getch();
        }
    } // end while(inInventory)

    clear();
    printw("You close your bag.\n");
    refresh();
}

void Game::explore_forest(Player& player, Map& map, vector<bool>& quest, AudioManager& audio) {
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
                move_character(player, 0, -1, map, quest, audio,player);
                break;

            case 's':
            case 'S':
                move_character(player, 0, 1, map, quest, audio,player);
                break;

            case 'a':
            case 'A':
                move_character(player, -1, 0, map, quest, audio,player);
                break;

            case 'd':
            case 'D':
                move_character(player, 1, 0, map, quest, audio,player);
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
                case 'i':
                runInventoryMenu(player,*this);
            default:
                add_log_message("Invalid key pressed.");
        }
        flushinp();
    }
}

void Game::game_loop(Player& player, AudioManager& audio) {
    clear();
    vector<bool> quest(5,false);
    Map stage1(player, quest, 153, 37, "../data/map/map.txt");
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
                explore_forest(player, stage1, quest, audio);
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
void Game::move_character(Character& entity, int x, int y, Map& map, vector<bool>& quest, AudioManager& audio, Player& player) {
    int newx = entity.get_x() + y;
    int newy = entity.get_y() + x;
    Tile* newTile = map.getTileAt(newx, newy);
    Tile* oldTile = map.getTileAt(entity.get_x(), entity.get_y());
    clear_dialogue_message();
    // --- 1. Check for Character on New Tile ---
    if (newTile->getCharacter() != nullptr) {
        clear_dialogue_message();
        
        Character* charOnTile = newTile->getCharacter();
        NPC* hattori_ptr = dynamic_cast<NPC*>(charOnTile);
        if (!newTile->getQuestStatus(quest)) {
        
        int enemyQuestID = newTile->get_doQuest();
        switch (enemyQuestID) 
        {
            case 0: // War Chief
                show_dialogue_message("Hattori says you must prove your strength. Defeat the War Chief first.Meet Hattori at (46,13)."); // <-- FIXED
                break;
            case 1: // Orc Raider
                show_dialogue_message("You are not powerful enough.Meet Hattori at (46,13)."); // <-- FIXED
                break;
            case 2: // Infernal Imp
                show_dialogue_message("You are not powerful enough.Meet Hattori at (46,13))."); // <-- FIXED
                break;
            case 3: 
                show_dialogue_message("The Golem's ancient magic is too strong. Complete other trials first.Meet Hattori at (46,13)."); // <-- FIXED
                break;
            case 4: 
                show_dialogue_message("A dark aura repels you. You must complete Hattori's other tasks first.Meet Hattori at (46,13).");
                break;
            case 5: // Final Boss
                show_dialogue_message("The Citadel is sealed. Hattori says you must defeat the bosses to enter.Meet Hattori at (46,13)."); // <-- FIXED
                break;
            default:
                show_dialogue_message("You are not powerful enough! Meet Hattori at (46,13)."); // <-- FIXED
                break;
        }
        return; // Stop the move
    }
        if (hattori_ptr) {
            clear_dialogue_message();
            std::vector<std::string> dialogue_lines;
            if (!quest[0]) { 
                dialogue_lines = hattori_ptr->give_quest_war_chief();
            } 
            else if (!quest[1]) { 
                dialogue_lines = hattori_ptr->give_quest_orc_raider();
            }
            else if (!quest[2]) { 
                dialogue_lines = hattori_ptr->give_quest_infernal_imp();
            }
            else if (!quest[3]) { // <-- FIXED
                dialogue_lines = hattori_ptr->give_quest_golem();
            }
            else if (!quest[4]) { // <-- FIXED
                dialogue_lines = hattori_ptr->give_quest_necromancer();
            }
            else if (quest[3] && quest[4]) {
                dialogue_lines = hattori_ptr->give_quest_final_boss();
            }
            else if (quest[5]) {
                 dialogue_lines = {hattori.getName() + ": 'You have saved us all, hero. Thank you.'" };
            }
            else {
                dialogue_lines = {hattori.getName() + ": 'Be strong, warrior. Great trials await.'" };
            }
        
            play_dialogue(dialogue_lines,player,map);
            
            return; // Stop the move
        }
        // --- IS IT AN ENEMY? (COMBAT CHECK) ---
        Combat c;
        if (!newTile->getQuestStatus(quest)) {
            // (Your existing quest-gate logic is good)
            int enemyQuestID = newTile->get_doQuest();
            // ... (your switch statement for "not powerful enough") ...
            return;
        }
        else{
            Combat c;
            if(!map.getTileAt(newx,newy)->getQuestStatus(quest)){
                add_log_message("You are not not powerful enough!");
                add_log_message("Meet Hattori at (46,13)");
                return;
            }
        Player& player = static_cast<Player&>(entity);
        Character* target_ptr = map.getTileAt(newx,newy)->getCharacter();
        Enemy& target = static_cast<Enemy&>(*target_ptr);
        add_log_message("Combat Triggered!");
        audio.playMusic("../data/audio/battle-bgm.mp3");
        int k = c.fight(player, target,*this);
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
            int questID = newTile->get_doQuest();
            if (questID != -1) 
            {
                quest[questID] = true;
                
                std::vector<std::string> complete_lines;
                
                // --- This switch "uses" your complete_quest functions ---
                switch (questID) {
                    case 0: complete_lines = this->hattori.complete_quest_war_chief(); break;
                    case 1: complete_lines = this->hattori.complete_quest_orc_raider(); break;
                    case 2: complete_lines = this->hattori.complete_quest_infernal_imp(); break;
                    case 3: complete_lines = this->hattori.complete_quest_golem(); break;
                    case 4: complete_lines = this->hattori.complete_quest_necromancer(); break;
                    case 5: complete_lines = this->hattori.complete_quest_final_boss(); break;
                }
\
                play_dialogue(complete_lines,player,map);
            }
            auto lootBox = target.getDropLoot();
            // --- 2. RUN THE LOOT MENU ---
            if (!lootBox.empty()) {
                add_log_message("Loot menu opened...");
                
                // Pass the lootBox by value (it gets moved in).
                // The menu returns what's left.
                std::vector<std::shared_ptr<Item>> remainingLoot = runLootMenu(player, std::move(lootBox));

                // remainingLoot now holds whatever the player left behind.
                // (Since we have "no tiles", these items are now gone)
                if (!remainingLoot.empty()) {
                    add_log_message("You left some items behind.");
                }

            } else {
                 add_log_message("The enemy dropped nothing.");
            }
            if(map.getTileAt(newx,newy)->get_doQuest()!=-1){
                quest[map.getTileAt(newx,newy)->get_doQuest()]=true;
            }
            auto lootBox = target.getDropLoot();
            // --- 2. RUN THE LOOT MENU ---
            if (!lootBox.empty()) {
                add_log_message("Loot menu opened...");
                
                // Pass the lootBox by value (it gets moved in).
                // The menu returns what's left.
                std::vector<std::shared_ptr<Item>> remainingLoot = runLootMenu(player, std::move(lootBox));

                // remainingLoot now holds whatever the player left behind.
                // (Since we have "no tiles", these items are now gone)
                if (!remainingLoot.empty()) {
                    add_log_message("You left some items behind.");
                }

            } else {
                 add_log_message("The enemy dropped nothing.");
            }
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
        audio.playMusic("../data/audio/sacred-garden-10377.mp3");
        return;
        }
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
        audio.playSFX("../data/audio/move-sfx.mp3");
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