#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <thread>
#include <chrono>
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

void Game::add_log_message(std::string message) {
    event_log.push_front(message);
    while (event_log.size() > MAX_LOG_LINES) {
        event_log.pop_back();
    }
}

// In Game.cpp
// (Make sure you have the DisplayItem struct here)

// Helper function to build the player's item list (same as before)
std::vector<DisplayItem> buildPlayerItemList(Player& player) {
    std::vector<DisplayItem> itemMap;

    // Add Equipment
    if (player.inventory.equippedWeapon) {
            itemMap.push_back({
                player.inventory.equippedWeapon->get_item_name() + " (Equipped)",
                "EQUIPPED_WEAPON", "WEAPON",player.inventory.equippedWeapon->get_item_description()
            });
        }
        if (player.inventory.equippedArmor) {
            itemMap.push_back({
                player.inventory.equippedArmor->get_item_name() + " (Equipped)",
                "EQUIPPED_ARMOR", "ARMOR",player.inventory.equippedArmor->get_item_description()
            });
        }

        // 2. Add Bag Items (using public members)
        if (player.inventory.inventoryWeapon) {
            itemMap.push_back({
                player.inventory.inventoryWeapon->get_item_name() + " (Bag)",
                "INVENTORY_WEAPON", "WEAPON",player.inventory.inventoryWeapon->get_item_description()
            });
        }
        if (player.inventory.inventoryArmor) {
            itemMap.push_back({
                player.inventory.inventoryArmor->get_item_name() + " (Bag)",
                "INVENTORY_ARMOR", "ARMOR",player.inventory.inventoryArmor->get_item_description()
            });
        }
        
        // 3. Add Potions
        for (const auto& pair : player.inventory.potionStorage) {
            if(pair.second.quantity>0)
            itemMap.push_back({
                pair.first + " (x" + to_string(pair.second.quantity) + ")",
                pair.first, "POTION", pair.second.itemPrototype->get_item_description()
            });
        }
        
    return itemMap;
}


/**
 * @brief This is the new (l)Loot / (d)Drop prompt-based loot menu.
 */
std::vector<std::shared_ptr<Item>> Game::runLootMenu(Player& player, 
                                                    std::vector<std::shared_ptr<Item>> lootBox) {
    bool inLootMenu = true;
    Game& world = *this; // For inventory functions

    while (inLootMenu) {
        clear();

        // --- 1. DISPLAY LOOT BOX (LEFT COLUMN) [1-9] ---
        mvprintw(0, 0, "--- LOOT BOX ---");
        int maxLootDisplay = 9; // Only show 1-9
        for (int i = 0; i < lootBox.size() && i < maxLootDisplay; ++i) {
            mvprintw(i + 2, 0, " [%d] %s", i + 1, lootBox[i]->get_item_name().c_str());
        }
        if (lootBox.empty()) {
            mvprintw(2, 0, " (Empty)");
        }

        // --- 2. DISPLAY PLAYER INVENTORY (RIGHT COLUMN) [a-z] ---
        mvprintw(0, 35, "--- YOUR INVENTORY ---");
        std::vector<DisplayItem> playerItems = buildPlayerItemList(player);
        for (int i = 0; i < playerItems.size() && i < 26; ++i) {
            mvprintw(i + 2, 35, " [%c] %s", 'a' + i, playerItems[i].displayName.c_str());
        }

        // --- 3. DISPLAY ACTIONS ---
        int row = std::max(12, (int)playerItems.size() + 4);
        mvprintw(row++, 0, "--- ACTIONS ---");
        mvprintw(row++, 0, "(l) Loot from Crate");
        mvprintw(row++, 0, "(d) Drop from Inventory");
        mvprintw(row++, 0, "(q) Quit Looting");
        mvprintw(row, 0, "Choice: ");
        refresh();

        int ch = getch();
        switch (ch) {
            case 'q':
            case 'Q':
                inLootMenu = false;
                break;

            // === LOOT FROM CRATE ===
            case 'l':
            case 'L': {
                mvprintw(row + 1, 0, "Loot item number (1-9): ");
                refresh();
                int ch_loot = getch();
                int index = ch_loot - '1'; // '1' -> 0

                if (index >= 0 && index < lootBox.size()) {
                    auto itemToTake = lootBox[index];
                    bool success = player.inventory.addItem(itemToTake, 1, player, world);
                    
                    if (success) {
                        lootBox.erase(lootBox.begin() + index); // Remove from loot box
                    } else {
                        mvprintw(row + 2, 0, "Inventory is full! Press any key...");
                        getch(); // Pause
                    }
                } else {
                    mvprintw(row + 2, 0, "Invalid number. Press any key...");
                    getch(); // Pause
                }
                break;
            }

            // === DROP FROM INVENTORY ===
            case 'd':
            case 'D': {
                mvprintw(row + 1, 0, "Drop item letter (a-z): ");
                refresh();
                int ch_drop = getch();
                int index = ch_drop - 'a'; // 'a' -> 0

                if (index >= 0 && index < playerItems.size()) {
                    // --- Show confirmation screen ---
                    DisplayItem itemToDrop = playerItems[index];
                    clear();
                    mvprintw(0, 0, "Drop %s?", itemToDrop.displayName.c_str());
                    mvprintw(2, 0, "Description: %s", itemToDrop.description.c_str());
                    mvprintw(4, 0, "(d) Drop");
                    mvprintw(5, 0, "(c) Cancel");
                    refresh();

                    int ch_confirm = getch();
                    if (ch_confirm == 'd' || ch_confirm == 'D') {
                        // User confirmed. Call the remove functions.
                        std::shared_ptr<Item> droppedItem = nullptr;
                        
                    if (itemToDrop.itemID == "EQUIPPED_WEAPON") {
                            // 1. Copy the pointer
                            droppedItem = player.inventory.equippedWeapon;
                            // 2. Call your existing void function
                            player.inventory.dropEquippedWeapon( world,player);
                        } 
                        else if (itemToDrop.itemID == "INVENTORY_WEAPON") {
                            droppedItem = player.inventory.inventoryWeapon;
                            player.inventory.dropInventoryWeapon(world);
                        } 
                        else if (itemToDrop.itemID == "EQUIPPED_ARMOR") {
                            droppedItem = player.inventory.equippedArmor;
                            player.inventory.dropEquippedArmor(world,player);
                        } 
                        else if (itemToDrop.itemID == "INVENTORY_ARMOR") {
                            droppedItem = player.inventory.inventoryArmor;
                            player.inventory.dropInventoryArmor(world);
                        } 
                        // --- OUR NEW LOGIC FOR POTIONS ---
                        else if (itemToDrop.type == "POTION") {
                            // Call the one new function
                            droppedItem = player.inventory.removePotionForLoot(itemToDrop.itemID, world);
                        }

                        // If we successfully removed it, add it to the loot box
                        if (droppedItem != nullptr) {
                            lootBox.push_back(droppedItem);
                        }
                    }
                    // If 'c' or anything else, do nothing and loop
                } else {
                    mvprintw(row + 2, 0, "Invalid letter. Press any key...");
                    getch(); // Pause
                }
                break;
            } // end case 'd'
        } // end switch
    } // end while

    // Return whatever is left in the loot box
    return lootBox;
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
                move_character(player, 0, -1, map, quest, audio);
                break;

            case 's':
            case 'S':
                move_character(player, 0, 1, map, quest, audio);
                break;

            case 'a':
            case 'A':
                move_character(player, -1, 0, map, quest, audio);
                break;

            case 'd':
            case 'D':
                move_character(player, 1, 0, map, quest, audio);
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

void Game::move_character(Character& entity, int x, int y, Map& map, vector<bool>& quest, AudioManager& audio) {
    int newx=entity.get_x()+y;
    int newy=entity.get_y()+x;
    if(map.getTileAt(newx,newy)->getCharacter() != nullptr) {
        if(map.getTileAt(newx,newy)->get_isNPC()){
            Character* npc_ptr = map.getTileAt(newx,newy)->getCharacter();
            NPC& npc = static_cast<NPC&>(*npc_ptr);
            npc.interact(quest);
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
        hpStepCount++;
        manaStepCount++;
        Player& player = static_cast<Player&>(entity);
        if(hpStepCount==player.getHPRegenStep()){
            hpStepCount=0;
            if(player.get_health()<player.get_max_health()){
                player.add_health(1);
                add_log_message("Regenerated 1 HP");
            }
        }
        if(manaStepCount==player.getManaRegenStep()){
            manaStepCount=0;
            if(player.get_mana()<player.get_max_mana()){
                player.add_mana(1);
                add_log_message("Regenerated 1 Mana");
            }
        }
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
        return;
    }
    else {
        add_log_message("You can't move there!");
        return;
    }
    add_log_message("Achievement Unlocked! The Void!");
}