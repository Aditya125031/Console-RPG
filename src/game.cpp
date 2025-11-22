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

using namespace std;

#include "../include/combat.hpp"
#include "../include/inventory.hpp"
#include "../include/game.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/map.h"
#include "../include/character.h"
#include "../include/items.h"
#include "../include/goblin.h"
#include "../include/npc.h"

class AudioManager; 

struct CreditLine
{
    std::string text;
    int color_pair;
    bool bold;
};
void Game::run_credits()
{
    clear();
    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);
    std::string exit_msg = "F A R E W E L L   T H E   U N D E F E A T E D.";
    mvprintw(term_height / 2, (term_width - exit_msg.length()) / 2, "%s", exit_msg.c_str());
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    std::string prompt = "[ Press any key to continue ]";
    mvprintw(rows - 2, (cols - prompt.length()) / 2, "%s", prompt.c_str());
    refresh();
    getch();
    clear();
    std::vector<CreditLine> credits = {
        {"", 0, false},
        {"==================================", 1, true},
        {"      THE DEFEATED      ", 2, true},
        {"==================================", 1, true},
        {"", 0, false},

        {"--- GAME DESIGN ---", 3, true},
        {"[ Team 7 ]", 4, false},
        {"", 0, false},

        {"--- CHARACTERS ---", 3, true},
        {"[ Aditya (Spartan) ]", 4, false},
        {"", 0, false},

        {"--- MAP ---", 3, true},
        {"[ Shuvam (Feonex) ]", 4, false},
        {"", 0, false},

        {"--- INVENTORY ---", 3, true},
        {"[ Mukund (Magnus Kalicharan)) ]", 4, false},
        {"", 0, false},

        {"--- ITEMS ---", 3, true},
        {"[ Arunoday (The_fool) ]", 4, false},
        {"", 0, false},

        {"--- COMBAT ---", 3, true},
        {"[ Ambadas (Locked_in)]", 4, false},
        {"", 0, false},

        {"--- SPECIAL THANKS ---", 3, true},
        {"pixabay - Sound", 5, false},
        {"Gemini - PDcurses", 5, false},
        {"", 0, false},

        {"==================================", 1, true},
        {"THANKS FOR PLAYING!", 2, true},
        {"==================================", 1, true},
        {"", 0, false}};

    curs_set(0);
    nodelay(stdscr, TRUE);

    int total_lines = credits.size();
    int start_row = term_height;

    for (int offset = 0; offset < term_height + total_lines; ++offset)
    {
        clear();

        for (int i = 0; i < total_lines; ++i)
        {
            int row = start_row + i - offset;

            if (row >= 0 && row < term_height)
            {
                std::string text = credits[i].text;
                int col = (term_width - text.length()) / 2;
                if (col < 0)
                    col = 0;

                if (credits[i].bold)
                    attron(A_BOLD);
                if (credits[i].color_pair > 0)
                    attron(COLOR_PAIR(credits[i].color_pair));

                mvprintw(row, col, "%s", text.c_str());

                if (credits[i].bold)
                    attroff(A_BOLD);
                if (credits[i].color_pair > 0)
                    attroff(COLOR_PAIR(credits[i].color_pair));
            }
        }

        refresh();

        int ch = getch();
        if (ch != ERR)
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(180));
    }

    nodelay(stdscr, FALSE);
    clear();
    curs_set(1);
}
void Game::play_cinematic_dialogue(const std::vector<std::string> &lines)
{
    clear();
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int max_width = cols - 10;
    std::vector<std::string> wrapped_lines;
    for (const std::string &line : lines)
    {
        std::vector<std::string> temp = wrap_text(line, max_width);
        wrapped_lines.insert(wrapped_lines.end(), temp.begin(), temp.end());
    }

    int start_y = (rows - wrapped_lines.size()) / 2;

    attron(A_BOLD);
    for (int i = 0; i < wrapped_lines.size(); ++i)
    {
        int text_len = wrapped_lines[i].length();
        int start_x = (cols - text_len) / 2;
        mvprintw(start_y + i, start_x, "%s", wrapped_lines[i].c_str());
        if (getch())
        {
            continue;
        }
    }
    attroff(A_BOLD);

    std::string prompt = "[ Press any key to continue ]";
    mvprintw(rows - 2, (cols - prompt.length()) / 2, "%s", prompt.c_str());

    refresh();

    nodelay(stdscr, FALSE);
    getch();
}

void Game::show_dialogue_message(const std::string &message)
{
    int term_width, term_height;
    getmaxyx(stdscr, term_height, term_width);
    int dialogue_width = term_width - 10;
    if (dialogue_width < 20)
        dialogue_width = 20;
    std::vector<std::string> wrapped_lines = wrap_text(message, dialogue_width);

    current_dialogue_lines.insert(current_dialogue_lines.end(), wrapped_lines.begin(), wrapped_lines.end());
}

void Game::add_log_message(std::string message)
{
    event_log.push_front(message);
    while (event_log.size() > MAX_LOG_LINES)
    {
        event_log.pop_back();
    }
}

std::vector<DisplayItem> Game::buildPlayerItemList(Player &player)
{
    std::vector<DisplayItem> itemMap;

    if (player.inventory.equippedWeapon)
    {
        itemMap.push_back({player.inventory.equippedWeapon->get_item_name() + " (Equipped)",
                           "EQUIPPED_WEAPON", "WEAPON", player.inventory.equippedWeapon->get_item_description() + " \nExtra Attack : " + to_string(player.inventory.equippedWeapon->get_weapon_attack()) + "\nExtra Mana : " + to_string(player.inventory.equippedWeapon->get_weapon_mana()) + "\nSpecial Description : " + player.inventory.equippedWeapon->get_sd() + "\n"});
    }
    if (player.inventory.equippedArmor)
    {
        itemMap.push_back({player.inventory.equippedArmor->get_item_name() + " (Equipped)",
                           "EQUIPPED_ARMOR", "ARMOR", player.inventory.equippedArmor->get_item_description() + " \nExtra Health : " + to_string(player.inventory.equippedArmor->get_armor_health()) + "\nExtra Mana : " + to_string(player.inventory.equippedArmor->get_armor_mana()) + "\n"});
    }

    if (player.inventory.inventoryWeapon)
    {
        itemMap.push_back({player.inventory.inventoryWeapon->get_item_name() + " (Bag)",
                           "INVENTORY_WEAPON", "WEAPON", player.inventory.inventoryWeapon->get_item_description() + " \nExtra Attack : " + to_string(player.inventory.inventoryWeapon->get_weapon_attack()) + "\nExtra Mana : " + to_string(player.inventory.inventoryWeapon->get_weapon_mana()) + "\nSpecial Description : " + player.inventory.inventoryWeapon->get_sd() + "\n"});
    }
    if (player.inventory.inventoryArmor)
    {
        itemMap.push_back({player.inventory.inventoryArmor->get_item_name() + " (Bag)",
                           "INVENTORY_ARMOR", "ARMOR", player.inventory.inventoryArmor->get_item_description() + " \nExtra Health : " + to_string(player.inventory.inventoryArmor->get_armor_health()) + "\nExtra Mana : " + to_string(player.inventory.inventoryArmor->get_armor_mana()) + "\n"});
    }

    for (const auto &pair : player.inventory.potionStorage)
    {
        if (pair.second.quantity > 0)
            itemMap.push_back({pair.first + " (x" + to_string(pair.second.quantity) + ")",
                               pair.first, "POTION", pair.second.itemPrototype->get_item_description() + "\nHeal Amount : " + to_string(pair.second.itemPrototype->get_usable_health()) + "\nRestores Mana : " + to_string(pair.second.itemPrototype->get_usable_mana()) + "\n"});
    }

    return itemMap;
}

std::vector<std::string> Game::wrap_text_v2(const std::string &text, int max_width)
{
    std::vector<std::string> final_lines;
    std::stringstream text_stream(text);
    std::string segment;

    bool first_segment = true;

    while (std::getline(text_stream, segment, '\n'))
    {
        if (segment.empty())
        {
            final_lines.push_back("");
            final_lines.push_back("");
            continue;
        }

        if (!first_segment)
        {
            final_lines.push_back("");
        }

        std::stringstream word_stream(segment);
        std::string word;
        std::string current_line;

        while (word_stream >> word)
        {
            if (current_line.empty() || current_line.length() + word.length() + 1 <= max_width)
            {
                if (!current_line.empty())
                {
                    current_line += " ";
                }
                current_line += word;
            }
            else
            {
                final_lines.push_back(current_line);
                current_line = word;
            }
        }

        if (!current_line.empty())
        {
            final_lines.push_back(current_line);
        }

        first_segment = false;
    }

    return final_lines;
}

vector<shared_ptr<Item>> Game::runLootMenu(Player &player,
                                           vector<shared_ptr<Item>> &lootBox)
{
    bool inLootMenu = true;
    Game &world = *this;

    enum class LootFocus
    {
        LOOT_BOX,
        INVENTORY
    };
    LootFocus currentFocus = LootFocus::LOOT_BOX;

    int loot_index = 0;
    int inventory_index = 0;

    clear();
    curs_set(0);
    int midY_anim = LINES / 2;
    int midX_anim = COLS / 2;
    attron(COLOR_PAIR(5));
    draw_box(midY_anim - 1, midX_anim - 10, 20, 2);
    mvprintw(midY_anim, midX_anim - 6, "Loot Found!");
    attroff(COLOR_PAIR(5));
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    while (inLootMenu)
    {
        clear();
        curs_set(0);

        vector<DisplayItem> playerItems = buildPlayerItemList(player);

        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        int topH = 15; 
        int descH = 12; 
        int totalUIHeight = topH + descH + 4;
        int startY = (rows - totalUIHeight) / 2;
        int startX = (cols - 80) / 2; 
        int w1 = 39;                
        int w2 = 39;                

        if (loot_index < 0)
            loot_index = lootBox.empty() ? 0 : lootBox.size() - 1; 
        if (loot_index >= lootBox.size())
            loot_index = 0; 
        if (inventory_index < 0)
            inventory_index = playerItems.empty() ? 0 : playerItems.size() - 1;
        if (inventory_index >= playerItems.size())
            inventory_index = 0; 

        attron(COLOR_PAIR(6));                                   
        draw_box(startY, startX, w1, topH);                     
        draw_box(startY, startX + w1 + 1, w2, topH);            
        draw_box(startY + topH + 1, startX, w1 + w2 + 1, descH);
        attroff(COLOR_PAIR(6));

        if (currentFocus == LootFocus::LOOT_BOX)
        {
            attron(COLOR_PAIR(1) | A_REVERSE);
            mvprintw(startY, startX + 2, "[ LOOT BOX ]");
            attroff(COLOR_PAIR(1) | A_REVERSE);
            attron(COLOR_PAIR(6) | A_DIM);
            mvprintw(startY, startX + w1 + 3, "[ YOUR INVENTORY ]");
            attroff(COLOR_PAIR(6) | A_DIM);
        }
        else
        {
            attron(COLOR_PAIR(6) | A_DIM);
            mvprintw(startY, startX + 2, "[ LOOT BOX ]");
            attroff(COLOR_PAIR(6) | A_DIM);
            attron(COLOR_PAIR(1) | A_REVERSE);
            mvprintw(startY, startX + w1 + 3, "[ YOUR INVENTORY ]");
            attroff(COLOR_PAIR(1) | A_REVERSE);
        }

        int itemY = startY + 2;
        int listMaxHeight = topH - 3;

        int itemX = startX + 2;
        if (lootBox.empty())
        {
            attron(A_DIM);
            mvprintw(itemY, itemX, "(Empty)");
            attroff(A_DIM);
        }
        else
        {
            for (int i = 0; i < lootBox.size() && i < listMaxHeight; ++i)
            {
                if (currentFocus == LootFocus::LOOT_BOX && i == loot_index)
                {
                    attron(COLOR_PAIR(5) | A_REVERSE);
                    mvprintw(itemY + i, itemX, "> %s", lootBox[i]->get_item_name().c_str());
                    attroff(COLOR_PAIR(5) | A_REVERSE);
                }
                else
                {
                    mvprintw(itemY + i, itemX + 2, "%s", lootBox[i]->get_item_name().c_str());
                }
            }
        }

        itemX = startX + w1 + 3;
        if (playerItems.empty())
        {
            attron(A_DIM);
            mvprintw(itemY, itemX, "(Empty)");
            attroff(A_DIM);
        }
        else
        {
            for (int i = 0; i < playerItems.size() && i < listMaxHeight; ++i)
            {
                if (currentFocus == LootFocus::INVENTORY && i == inventory_index)
                {
                    attron(COLOR_PAIR(5) | A_REVERSE);
                    mvprintw(itemY + i, itemX, "> %s", playerItems[i].displayName.c_str());
                    attroff(COLOR_PAIR(5) | A_REVERSE);
                }
                else
                {
                    mvprintw(itemY + i, itemX + 2, "%s", playerItems[i].displayName.c_str());
                }
            }
        }

        int descY = startY + topH + 2;
        int descX = startX + 2;
        int descWidth = w1 + w2 - 2;
        std::string desc_text = "Select an item to see its description.";

        if (currentFocus == LootFocus::LOOT_BOX && !lootBox.empty())
        {
            desc_text = lootBox[loot_index]->get_item_description();
        }
        else if (currentFocus == LootFocus::INVENTORY && !playerItems.empty())
        {
            desc_text = playerItems[inventory_index].description;
        }

        attron(COLOR_PAIR(3) | A_BOLD);
        mvprintw(descY, descX, "Description:");
        attroff(COLOR_PAIR(3) | A_BOLD);

        std::vector<std::string> wrapped_lines = wrap_text_v2(desc_text, descWidth);
        attron(COLOR_PAIR(6));
        for (int i = 0; i < wrapped_lines.size() && i < (descH - 2); ++i)
        {
            mvprintw(descY + 1 + i, descX, "%s", wrapped_lines[i].c_str());
        }
        attroff(COLOR_PAIR(6));

        int actionY = startY + topH + descH + 3;
        mvprintw(actionY, startX, "[ARROWS/WASD] Navigate | [q] Quit");

        if (currentFocus == LootFocus::LOOT_BOX && !lootBox.empty())
        {
            attron(COLOR_PAIR(2) | A_BOLD); 

            mvprintw(actionY, startX + 40, "[SPACE] Loot All");
            mvprintw(actionY, startX + 60, "[ENTER] Loot");

            attroff(COLOR_PAIR(2) | A_BOLD);
        }
        else if (currentFocus == LootFocus::INVENTORY && !playerItems.empty())
        {
            attron(COLOR_PAIR(4) | A_BOLD); 
            mvprintw(actionY, startX + 60, "[ENTER] Drop");
            attroff(COLOR_PAIR(4) | A_BOLD);
        }

        refresh();

        int ch = getch();
        switch (ch)
        {
        case 'q':
        case 'Q':
            inLootMenu = false;
            break;

        case KEY_UP:
        case 'w':
            if (currentFocus == LootFocus::LOOT_BOX)
            {
                loot_index--;
            }
            else
            {
                inventory_index--;
            }
            break;

        case KEY_DOWN:
        case 's':
            if (currentFocus == LootFocus::LOOT_BOX)
            {
                loot_index++;
            }
            else
            {
                inventory_index++;
            }
            break;

        case KEY_LEFT:
        case 'a':
            currentFocus = (currentFocus == LootFocus::LOOT_BOX) ? LootFocus::INVENTORY : LootFocus::LOOT_BOX;
            break;

        case KEY_RIGHT:
        case 'd':
            currentFocus = (currentFocus == LootFocus::LOOT_BOX) ? LootFocus::INVENTORY : LootFocus::LOOT_BOX;
            break;

        case 10: 
            if (currentFocus == LootFocus::LOOT_BOX && !lootBox.empty())
            {
                auto itemToTake = lootBox[loot_index];
                bool success = player.inventory.addItem(itemToTake, 1, player, world);

                if (success)
                {
                    lootBox.erase(lootBox.begin() + loot_index);
                }
                else
                {
                    int popW = 30;
                    int popH = 4;
                    int popY = (LINES - popH) / 2;
                    int popX = (COLS - popW) / 2;

                    attron(COLOR_PAIR(4) | A_BOLD); 
                    mvprintw(popY + 20, popX + (popW - 19) / 2, "Inventory is full!");
                    attroff(COLOR_PAIR(4) | A_BOLD);

                    refresh();
                    getch(); 
                }
            }
            else if (currentFocus == LootFocus::INVENTORY && !playerItems.empty())
            {
                DisplayItem itemToDrop = playerItems[inventory_index];

                shared_ptr<Item> droppedItem = nullptr;
                if (itemToDrop.itemID == "EQUIPPED_WEAPON")
                {
                    droppedItem = player.inventory.equippedWeapon;
                    player.inventory.dropEquippedWeapon(world, player);
                }
                else if (itemToDrop.itemID == "INVENTORY_WEAPON")
                {
                    droppedItem = player.inventory.inventoryWeapon;
                    player.inventory.dropInventoryWeapon(world);
                }
                else if (itemToDrop.itemID == "EQUIPPED_ARMOR")
                {
                    droppedItem = player.inventory.equippedArmor;
                    player.inventory.dropEquippedArmor(world, player);
                }
                else if (itemToDrop.itemID == "INVENTORY_ARMOR")
                {
                    droppedItem = player.inventory.inventoryArmor;
                    player.inventory.dropInventoryArmor(world);
                }
                else if (itemToDrop.type == "POTION")
                {
                    droppedItem = player.inventory.removePotionForLoot(itemToDrop.itemID, world);
                }

                if (droppedItem != nullptr)
                {
                    lootBox.push_back(droppedItem);
                }
            }
            break;

        case ' ':
            if (currentFocus == LootFocus::LOOT_BOX && !lootBox.empty())
            {
                for (size_t i = 0; i < lootBox.size();)
                {
                    auto itemToTake = lootBox[i];
                    bool success = player.inventory.addItem(itemToTake, 1, player, world);

                    if (success)
                    {
                        lootBox.erase(lootBox.begin() + i);
                    }
                    else
                    {
                        int popW = 30;
                        int popH = 4;
                        int popY = (LINES - popH) / 2; 
                        int popX = (COLS - popW) / 2;  

                        attron(COLOR_PAIR(4) | A_BOLD);
                        draw_box(popY, popX, popW, popH);
                        mvprintw(popY + 1, popX + (popW - 19) / 2, "Inventory is full!");
                        attroff(COLOR_PAIR(4) | A_BOLD);

                        attron(A_DIM);
                        mvprintw(popY + 2, popX + (popW - 18) / 2, "Press any key...");
                        attroff(A_DIM);

                        refresh();
                        getch(); 
                        break;  
                    }
                }
                loot_index = 0;
            }
            break;
        } 
    } 

    return lootBox;
}

void Game::clear_dialogue_message()
{
    current_dialogue_lines.clear();
}

bool Game::showGameOverScreen(AudioManager &audio)
{
    audio.playMusic("../data/audio/game-over.mp3");
    nodelay(stdscr, FALSE);
    curs_set(0);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    const std::vector<std::string> title = {
        "  ____   _____ _____  _____     _   _____ _____  ____  ",
        " |  _ \\ | ____|  ___|| ____|   / \\ |_   _| ____||  _ \\ ",
        " | | | ||  _| | |_   |  _|    / _ \\  | | |  _|  | | | |",
        " | |_| || |___|  _|  | |___  / ___ \\ | | | |___ | |_| |",
        " |____/ |_____|_|    |_____|/_/   \\_\\|_| |_____||____/ ",
        "                                                  ",
        "                         O N C E    A G A I N . . .             "};

    int startY = (rows / 2) - 8;
    int startX = (cols - title[0].length()) / 2;

    const std::string options[2] = {
        "Retry from Last Checkpoint",
        "Exit Game"};
    int choice = 0;

    while (true)
    {
        clear();
        attron(COLOR_PAIR(4));
        for (int x = 0; x < cols; x++)
        {
            mvaddch(0, x, ACS_CKBOARD);
            mvaddch(rows - 1, x, ACS_CKBOARD);
        }
        for (int y = 0; y < rows; y++)
        {
            mvaddch(y, 0, ACS_CKBOARD);
            mvaddch(y, cols - 1, ACS_CKBOARD);
        }
        attroff(COLOR_PAIR(4));

        attron(COLOR_PAIR(4) | A_BOLD);
        for (size_t i = 0; i < title.size(); ++i)
        {
            mvprintw(startY + i, startX, "%s", title[i].c_str());
        }
        attroff(COLOR_PAIR(4) | A_BOLD);

        int menuY = startY + title.size() + 4;
        for (int i = 0; i < 2; ++i)
        {
            int optLen = options[i].length();
            int optX = (cols - optLen) / 2;

            if (i == choice)
            {
                attron(A_REVERSE | A_BOLD);
                mvprintw(menuY + (i * 2), optX - 2, "> %s <", options[i].c_str());
                attroff(A_REVERSE | A_BOLD);
            }
            else
            {
                mvprintw(menuY + (i * 2), optX, "%s", options[i].c_str());
            }
        }

        refresh();

        int ch = getch();
        switch (ch)
        {
        case 'w':
        case 'W':
        case KEY_UP:
            choice--;
            if (choice < 0)
                choice = 1;
            break;
        case 's':
        case 'S':
        case KEY_DOWN:
            choice++;
            if (choice > 1)
                choice = 0;
            break;
        case 10:
            if (choice == 0)
                return true;
            else
                return false;
            break;
        }
    }
}

void Game::display_dashboard(Player &player, Map &map)
{
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

    mvprintw(row, left_padding, "%s", title_prefix.c_str());
    attron(COLOR_PAIR(4) | A_BOLD); 
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

    mvprintw(row, 0, " HP: ");
    attron(COLOR_PAIR(2));
    printw("%d/%d", player.get_health(), player.get_max_health());
    attroff(COLOR_PAIR(2));
    row++;

    mvprintw(row, 0, " Attack: ");
    attron(COLOR_PAIR(2));
    printw("%d", player.get_attack_power());
    attroff(COLOR_PAIR(2));
    row++;

    mvprintw(row, 0, " Mana: ");
    attron(COLOR_PAIR(1));
 
    printw("%d/%d", player.get_mana(), player.get_max_mana());
    attroff(COLOR_PAIR(1));
    row++;
    mvprintw(row++, 0, "────────────────────────────────────────────────────────");

    row += 12;
    mvprintw(row++, 2, "[ EQUIPMENT ]");
    if (player.inventory.equippedWeapon)
    {
        mvprintw(row++, 2, " W: %s", player.inventory.equippedWeapon->get_item_name().c_str());
    }
    else
    {
        mvprintw(row++, 2, " W: (None)");
    }
    if (player.inventory.equippedArmor)
    {
        mvprintw(row++, 2, " A: %s", player.inventory.equippedArmor->get_item_name().c_str());
    }
    else
    {
        mvprintw(row++, 2, " A: (None)");
    }

    int minimap_height = 17;
    int minimap_width = 29;
    map.get_minimap_view(player, minimap_width, minimap_height, event_log);
    mvprintw(term_height - 3, 0, " CONTROLS: (W/A/S/D) Move | (M) Full Map | (Q) Quit to Village | (I) Inventory ");
    mvprintw(term_height - 2, 0, " Your Location: (%d, %d)", player.get_y(), player.get_x());
    int dialogue_Y_line = 35;
    int i = 0;
    for (const std::string &line : current_dialogue_lines)
    {
        int start_x = (term_width - line.length()) / 2;
        if (start_x < 0)
        {
            start_x = 0;
        }
        mvprintw(dialogue_Y_line + i, start_x, line.c_str());
        i++;
    }
    refresh();
}
std::vector<std::string> Game::wrap_text(const std::string &text, int max_width)
{
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string word;
    std::string current_line;

    while (ss >> word)
    {
        if (current_line.empty() || current_line.length() + word.length() + 1 <= max_width)
        {
            if (!current_line.empty())
            {
                current_line += " ";
            }
            current_line += word;
        }
        else
        {
            lines.push_back(current_line);
            current_line = word;
        }
    }
    if (!current_line.empty())
    {
        lines.push_back(current_line);
    }
    return lines;
}
void Game::play_dialogue(const std::vector<std::string> &lines, Player &player, Map &map)
{

    int term_width, term_height;
    getmaxyx(stdscr, term_height, term_width);

    int dialogue_width = term_width - 10;
    if (dialogue_width < 20)
        dialogue_width = 20;
    std::vector<std::string> all_wrapped_lines;
    for (const std::string &original_line : lines)
    {
        std::vector<std::string> wrapped = wrap_text(original_line, dialogue_width);
        all_wrapped_lines.insert(all_wrapped_lines.end(), wrapped.begin(), wrapped.end());
    }

    for (const std::string &line : all_wrapped_lines)
    {
        current_dialogue_lines.push_back(line);
        display_dashboard(player, map);
        getch();
    }
}

void Game::show_full_map(Map &map)
{
    bool onFullMap = true;

    while (onFullMap)
    {
        map.render();
        if (is_termresized())
        {
            resize_term(0, 0);
            clear();
            refresh();
            continue;
        }
        int input = getch();

        switch (input)
        {
        case 'm':
        case 'M':
            onFullMap = false;
            break;

        default:
            break;
        }
        refresh();
    }

    clear();
}

void Game::runInventoryMenu(Player &player, Game &world)
{
    clear();
    curs_set(0);
    int midY = LINES / 2;
    int midX = COLS / 2;
    int boxW = 28;
    int boxH = 2;

    attron(COLOR_PAIR(5));
    draw_box(midY - 1, midX - (boxW / 2), boxW, boxH);
    mvprintw(midY, midX - 10, "Opening Inventory...");
    attroff(COLOR_PAIR(5));

    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    bool inInventory = true;
    int selected_index = 0;

    while (inInventory)
    {
        clear();
        curs_set(0);

        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        int boxW = 70;                    
        int boxH = 18;                   
        int startY = (rows - boxH) / 2 - 2; 
        int startX = (cols - boxW) / 2;

        attron(COLOR_PAIR(6)); 
        draw_box(startY, startX, boxW, boxH);
        attroff(COLOR_PAIR(6));

        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(startY - 1, startX - 3 + (boxW - 13) / 2, "--- INVENTORY ---");
        attroff(COLOR_PAIR(1) | A_BOLD);

        int splitX = startX + (boxW / 2) - 2;
        attron(COLOR_PAIR(6));
        mvaddch(startY, splitX, ACS_TTEE);
        mvaddch(startY + boxH, splitX, ACS_BTEE);
        mvvline(startY + 1, splitX, ACS_VLINE, boxH - 1);
        attroff(COLOR_PAIR(6));

        vector<DisplayItem> itemMap = buildPlayerItemList(player);
        int itemY = startY + 2;
        int itemX = startX + 2;

        DisplayItem selectedItem; 

        if (itemMap.empty())
        {
            attron(A_DIM);
            mvprintw(itemY, itemX, "(Empty)");
            attroff(A_DIM);
        }
        else
        {
            if (selected_index < 0)
                selected_index = 0;
            if (selected_index >= itemMap.size())
                selected_index = itemMap.size() - 1;

            selectedItem = itemMap[selected_index];

            for (int i = 0; i < itemMap.size(); ++i)
            {
                std::string item_name = itemMap[i].displayName;
                int max_name_width = (splitX - itemX) - 4;
                if (item_name.length() > max_name_width)
                {
                    item_name = item_name.substr(0, max_name_width) + "...";
                }

                if (i == selected_index)
                {
                    attron(COLOR_PAIR(5) | A_REVERSE);
                    mvprintw(itemY + i, itemX, "> %s", item_name.c_str());
                    attroff(COLOR_PAIR(5) | A_REVERSE);
                }
                else
                {
                    mvprintw(itemY + i, itemX + 2, "%s", item_name.c_str());
                }
            }
        }

        int descY = startY + 2;
        int descX = splitX + 2;
        int descWidth = (startX + boxW) - descX - 2;

        if (!itemMap.empty())
        {
            attron(COLOR_PAIR(3) | A_BOLD);
            mvprintw(descY, descX, "Item Description:");
            attroff(COLOR_PAIR(3) | A_BOLD);

            std::string desc_text = selectedItem.description;
            std::vector<std::string> wrapped_lines = wrap_text_v2(desc_text, descWidth);

            attron(COLOR_PAIR(6));
            for (int i = 0; i < wrapped_lines.size() && i < (boxH - 4); ++i)
            {
                mvprintw(descY + 2 + i, descX, "%s", wrapped_lines[i].c_str());
            }
            attroff(COLOR_PAIR(6));
        }
        else
        {
            mvprintw(descY, descX, "Your bag is empty.");
        }

        int actionY = startY + boxH + 1;

        mvprintw(actionY, startX, "[UP/DOWN] Navigate");
        mvprintw(actionY + 1, startX, "[q] Quit");

        if (!itemMap.empty())
        {
            attron(COLOR_PAIR(5) | A_BOLD);

            if (selectedItem.type == "WEAPON" || selectedItem.type == "ARMOR")
            {
                mvprintw(actionY, startX + 25, "[ENTER] Equip/Swap");
            }
            else if (selectedItem.type == "POTION")
            {
                mvprintw(actionY, startX + 25, "[ENTER] Use");
            }

            mvprintw(actionY + 1, startX + 25, "[d] Drop %s", selectedItem.displayName.c_str());

            attroff(COLOR_PAIR(5) | A_BOLD);
        }

        actionY += 2; 

        attron(COLOR_PAIR(6));
        mvaddch(actionY, startX, ACS_LTEE);
        mvhline(actionY, startX + 1, ACS_HLINE, boxW - 2);
        mvaddch(actionY, startX + boxW, ACS_RTEE);
        attroff(COLOR_PAIR(6));

        refresh();

        int ch = getch();
        switch (ch)
        {
        case 'q':
        case 'Q':
            inInventory = false;
            continue;

        case KEY_UP:
        case 'w':
            selected_index--;
            if (selected_index < 0)
                selected_index = itemMap.empty() ? 0 : itemMap.size() - 1;
            break;

        case KEY_DOWN:
        case 's':
            selected_index++;
            if (itemMap.empty() || selected_index >= itemMap.size())
                selected_index = 0;
            break;

        case 10:
        {
            if (itemMap.empty())
                break;

            if (selectedItem.type == "WEAPON" || selectedItem.type == "ARMOR")
            {
                if (selectedItem.type == "WEAPON")
                {
                    player.inventory.swapWeapon(player, world);
                }
                else if (selectedItem.type == "ARMOR")
                {
                    player.inventory.swapArmor(player, world);
                }
            }
            else if (selectedItem.type == "POTION")
            {
                bool inConfirmation = true;
                bool didConfirm = false;
                int confirm_choice = 1;

                int popW = 40;
                int popH = 6;
                int popY = (LINES - popH) / 2;
                int popX = (COLS - popW) / 2;

                while (inConfirmation)
                {
                
                    clear();                  
                    attron(COLOR_PAIR(2) | A_BOLD);
                    draw_box(popY, popX, popW, popH);
                    attroff(COLOR_PAIR(2) | A_BOLD);

                    std::string use_prompt = "Use " + selectedItem.displayName + "?";
                    mvprintw(popY + 1, popX + (popW - use_prompt.length()) / 2, "%s", use_prompt.c_str());

                    attron(A_DIM);
                    std::string desc_line = "(This will consume the item)";
                    mvprintw(popY + 2, popX + (popW - desc_line.length()) / 2, "%s", desc_line.c_str());
                    attroff(A_DIM);

                    if (confirm_choice == 0)
                    {
                        attron(COLOR_PAIR(6) | A_REVERSE);
                        mvprintw(popY + 4, popX + 8, "[ YES ]");
                        attroff(COLOR_PAIR(6) | A_REVERSE);

                        mvprintw(popY + 4, popX + 26, "  NO  ");
                    }
                    else
                    { 
                        mvprintw(popY + 4, popX + 8, "  YES  ");

                        attron(COLOR_PAIR(6) | A_REVERSE);
                        mvprintw(popY + 4, popX + 26, "[ NO ]");
                        attroff(COLOR_PAIR(6) | A_REVERSE);
                    }

                    refresh();

                    int ch_confirm = getch();
                    switch (ch_confirm)
                    {
                    case KEY_LEFT:
                    case 'a':
                        confirm_choice = 0; 
                        break;
                    case KEY_RIGHT:
                    case 'd':
                        confirm_choice = 1;
                        break;
                    case 10: 
                        if (confirm_choice == 0)
                        {
                            didConfirm = true;
                        }
                        inConfirmation = false;
                        break;
                    case 'q':
                    case 'c': 
                        inConfirmation = false;
                        break;
                    }
                }

                if (didConfirm)
                {
                    player.inventory.usePotion(selectedItem.itemID, player, world);
                }
            }

            itemMap = buildPlayerItemList(player);
            if (selected_index >= itemMap.size())
                selected_index = itemMap.size() - 1;
            if (selected_index < 0)
                selected_index = 0;
        }
        break;

        case 'd':
        case 'D':
        {
            if (itemMap.empty())
                break; 

            bool inConfirmation = true;
            bool didConfirm = false;
            int confirm_choice = 1; 

            int popW = 40;
            int popH = 6;
            int popY = (LINES - popH) / 2;
            int popX = (COLS - popW) / 2;

            while (inConfirmation)
            {
                clear();                     
                attron(COLOR_PAIR(4) | A_BOLD); 
                draw_box(popY, popX, popW, popH);
                attroff(COLOR_PAIR(4) | A_BOLD);

                std::string drop_prompt = "Drop 1 " + selectedItem.displayName + "?";
                mvprintw(popY + 1, popX + (popW - drop_prompt.length()) / 2, "%s", drop_prompt.c_str());

                attron(A_DIM);
                std::string desc_line = "(This action is permanent)";
                mvprintw(popY + 2, popX + (popW - desc_line.length()) / 2, "%s", desc_line.c_str());
                attroff(A_DIM);

                if (confirm_choice == 0)
                {
                    attron(COLOR_PAIR(6) | A_REVERSE);
                    mvprintw(popY + 4, popX + 8, "[ YES ]");
                    attroff(COLOR_PAIR(6) | A_REVERSE);

                    mvprintw(popY + 4, popX + 26, "  NO  ");
                }
                else
                {
                    mvprintw(popY + 4, popX + 8, "  YES  ");

                    attron(COLOR_PAIR(6) | A_REVERSE);
                    mvprintw(popY + 4, popX + 26, "[ NO ]");
                    attroff(COLOR_PAIR(6) | A_REVERSE);
                }

                refresh();

                int ch_confirm = getch();
                switch (ch_confirm)
                {
                case KEY_LEFT:
                case 'a':
                    confirm_choice = 0;
                    break;
                case KEY_RIGHT:
                case 'd':
                    confirm_choice = 1;
                    break;
                case 10:
                    if (confirm_choice == 0)
                    {
                        didConfirm = true;
                    }
                    inConfirmation = false;
                    break;
                case 'q':
                case 'c':
                    inConfirmation = false;
                    break;
                }
            }

            if (didConfirm)
            {
                if (selectedItem.itemID == "EQUIPPED_WEAPON")
                {
                    player.inventory.dropEquippedWeapon(world, player);
                }
                else if (selectedItem.itemID == "INVENTORY_WEAPON")
                {
                    player.inventory.dropInventoryWeapon(world);
                }
                else if (selectedItem.itemID == "EQUIPPED_ARMOR")
                {
                    player.inventory.dropEquippedArmor(world, player);
                }
                else if (selectedItem.itemID == "INVENTORY_ARMOR")
                {
                    player.inventory.dropInventoryArmor(world);
                }
                else if (selectedItem.type == "POTION")
                {
                    player.inventory.dropPotion(selectedItem.itemID, world);
                }

                itemMap = buildPlayerItemList(player); 
                if (selected_index >= itemMap.size())
                    selected_index = itemMap.size() - 1;
                if (selected_index < 0)
                    selected_index = 0;
            }
        }
        break;
        }
    } 
    clear();
    curs_set(0);
    midY = LINES / 2;
    midX = COLS / 2;
    boxW = 22;
    boxH = 2;

    attron(COLOR_PAIR(6) | A_DIM);
    draw_box(midY - 1, midX - (boxW / 2), boxW, boxH);
    mvprintw(midY, midX - 8, "Inventory Closed");
    attroff(COLOR_PAIR(6) | A_DIM);

    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void Game::explore_forest(Player &player, Map &map, vector<bool> &quest, AudioManager &audio)
{
    add_log_message("You entered the forest.");

    nodelay(stdscr, TRUE);

    lastHpRegenTime = std::chrono::steady_clock::now();
    lastManaRegenTime = std::chrono::steady_clock::now();
    bool inForest = true;

    while (inForest)
    {
        auto now = std::chrono::steady_clock::now();

        auto elapsedHpSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - lastHpRegenTime).count();
        int hpInterval = player.getHPRegenTime(); 

        if (elapsedHpSeconds >= hpInterval && hpInterval > 0)
        {
            int healAmount = 1;
            if (player.get_health() < player.get_max_health())
            {
                player.add_health(healAmount);
                add_log_message("Regenerated " + std::to_string(healAmount) + " HP");
            }
            lastHpRegenTime = now;
        }

        auto elapsedManaSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - lastManaRegenTime).count();
        int manaInterval = player.getManaRegenTime();

        if (elapsedManaSeconds >= manaInterval && manaInterval > 0)
        {
            int manaAmount = 1;
            if (player.get_mana() < player.get_max_mana())
            {
                player.add_mana(manaAmount);
                add_log_message("Regenerated " + std::to_string(manaAmount) + " Mana");
            }
            lastManaRegenTime = now;
        }

        display_dashboard(player, map);

        if (is_termresized())
        {
            resize_term(0, 0);
            clear();
            refresh();
            continue;
        }

        int input = getch();

        if (input == ERR)
        {
            napms(50); 
            continue;
        }

        nodelay(stdscr, FALSE);

        switch (input)
        {
        case 'w':
        case 'W':
        case KEY_UP:
            move_character(player, 0, -1, map, quest, audio, player);
            break;
        case 's':
        case 'S':
        case KEY_DOWN:
            move_character(player, 0, 1, map, quest, audio, player);
            break;
        case 'a':
        case 'A':
        case KEY_LEFT:
            move_character(player, -1, 0, map, quest, audio, player);
            break;
        case 'd':
        case 'D':
        case KEY_RIGHT:
            move_character(player, 1, 0, map, quest, audio, player);
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
        case 'I':
            runInventoryMenu(player, *this);
            break;
        default:
            break;
        }

        if (inForest)
        {
            nodelay(stdscr, TRUE);
        }
        this_thread::sleep_for(std::chrono::milliseconds(100));
        flushinp();
    }

    nodelay(stdscr, FALSE);
}

void Game::draw_box(int y, int x, int w, int h)
{
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y, x + w, ACS_URCORNER);
    mvaddch(y + h, x, ACS_LLCORNER);
    mvaddch(y + h, x + w, ACS_LRCORNER);
    mvhline(y, x + 1, ACS_HLINE, w - 1);
    mvhline(y + h, x + 1, ACS_HLINE, w - 1);
    mvvline(y + 1, x, ACS_VLINE, h - 1);
    mvvline(y + 1, x + w, ACS_VLINE, h - 1);
}

void Game::mvprintw_center(int y, const std::string &text)
{
    int x = (COLS - text.length()) / 2;
    mvprintw(y, x, "%s", text.c_str());
}


void Game::game_loop(Player &player, AudioManager &audio)
{
    clear();
    vector<bool> quest(6, false);
    Map stage1(player, quest, 153, 37, "../data/map/map.txt");

    int totalDuration = 1500; 
    int interval = 50;      
    int elapsed = 0;

    attron(COLOR_PAIR(6)); 
    mvprintw_center(LINES / 2, "You find your way to a nearby village to rest.");
    attroff(COLOR_PAIR(6));
    refresh();

    while (elapsed < totalDuration)
    {
        if (is_termresized())
        {
            resize_term(0, 0);
            clear();
            attron(COLOR_PAIR(6));
            mvprintw_center(LINES / 2, "You find your way to a nearby village to rest.");
            attroff(COLOR_PAIR(6));
            refresh();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        elapsed += interval;
    }
    flushinp();

    bool isGameRunning = true;
    int choice = 0; 
    const std::vector<std::string> options = {
        "Show My Stats",
        "Venture into the forest",
        "Exit Game"};

    while (isGameRunning)
    {
        clear();
        curs_set(0);

        attron(COLOR_PAIR(5) | A_BOLD); 
        mvprintw_center(LINES / 2 - 8, "~~~ THE VILLAGE OF RESPITE ~~~");
        attroff(COLOR_PAIR(5) | A_BOLD);

        for (int i = 0; i < options.size(); ++i)
        {
            int boxY = (LINES / 2) - 4 + (i * 3);
            int boxX = (COLS / 2) - 20;

            if (i == choice)
            {
                attron(COLOR_PAIR(1) | A_BOLD);
                draw_box(boxY - 1, boxX - 2, 44, 2);
                mvprintw(boxY, boxX, ">> %s <<", options[i].c_str());
                attroff(COLOR_PAIR(1) | A_BOLD);
            }
            else
            {
                attron(COLOR_PAIR(6)); 
                mvprintw(boxY, boxX, "   %s   ", options[i].c_str());
                attroff(COLOR_PAIR(6));
            }
        }

        mvprintw_center(LINES - 3, "Use UP/DOWN keys and ENTER to select.");
        refresh();

        if (is_termresized())
        {
            resize_term(0, 0);
            clear();
            refresh();
            continue;
        }

        int menuChoice = getch();
        switch (menuChoice)
        {
        case KEY_UP:
        case 'w':
            choice--;
            if (choice < 0)
                choice = options.size() - 1;
            break;
        case KEY_DOWN:
        case 's':
            choice++;
            if (choice >= options.size())
                choice = 0;
            break;

        case 10: 
         
            if (choice == 0)
            {
                clear();
                curs_set(1);
                player.show_details();
                mvprintw(getmaxy(stdscr) - 2, 0, "Press any key to return...");
                refresh();
                getch();
                flushinp();
            }
            else if (choice == 1)
            {
                explore_forest(player, stage1, quest, audio);
                if (!player.isAlive())
                {
                    isGameRunning = false;
                }
            }
            else if (choice == 2)
            {
                attron(COLOR_PAIR(6));
                mvprintw_center(LINES - 1, "Thanks for playing!");
                attroff(COLOR_PAIR(6));
                refresh();
                this_thread::sleep_for(chrono::seconds(1));
                flushinp();
                isGameRunning = false;
            }
            break;

        default:
            break;
        }
    }
}

void Game::move_character(Character &entity, int x, int y, Map &map, vector<bool> &quest, AudioManager &audio, Player &player)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int newx = entity.get_x() + y;
    int newy = entity.get_y() + x;
    Tile *newTile = map.getTileAt(newx, newy);
    Tile *oldTile = map.getTileAt(entity.get_x(), entity.get_y());
    clear_dialogue_message();
    // --- 1. Check for Character on New Tile ---
    if (newTile->getCharacter() != nullptr)
    {
        clear_dialogue_message();

        Character *charOnTile = newTile->getCharacter();
        NPC *hattori_ptr = dynamic_cast<NPC *>(charOnTile);
        if (hattori_ptr)
        {
            clear_dialogue_message();
            std::vector<std::string> dialogue_lines;
            if (!quest[0])
            {
                dialogue_lines = hattori_ptr->give_quest_war_chief();
            }
            else if (!quest[1])
            {
                dialogue_lines = hattori_ptr->give_quest_orc_raider();
            }
            else if (!quest[2])
            {
                dialogue_lines = hattori_ptr->give_quest_infernal_imp();
            }
            else if (!quest[3])
            { // <-- FIXED
                dialogue_lines = hattori_ptr->give_quest_golem();
            }
            else if (!quest[4])
            { // <-- FIXED
                dialogue_lines = hattori_ptr->give_quest_necromancer();
            }
            else if (quest[3] && quest[4])
            {
                dialogue_lines = hattori_ptr->give_quest_final_boss();
            }
            else
            {
                dialogue_lines = {hattori.getName() + ": 'Be strong, warrior. Great trials await.'"};
            }

            play_dialogue(dialogue_lines, player, map);

            return; 
        }
        if (!newTile->getQuestStatus(quest))
        {

            int enemyQuestID = newTile->getRequiredQuestCompleted();
            switch (enemyQuestID)
            {
            case -1:
                break;
            case 0:                                                                                                                    // War Chief
                show_dialogue_message("Oracle says you must prove your strength. Defeat the War Chief first.Meet Oracle at (46,13)."); // <-- FIXED
                break;
            case 1:                                                                           // Orc Raider
                show_dialogue_message("You are not powerful enough.Meet Oracle at (46,13)."); // <-- FIXED
                break;
            case 2:
                show_dialogue_message("The Golem's ancient magic is too strong. Complete other trials first.Meet Oracle at (46,13)."); // <-- FIXED
                break;
            case 3:
                show_dialogue_message("A dark aura repels you. You must complete Oracle's other tasks first. Meet Oracle at (46,13).");
                break;
            case 4:                                                                                                                      // Final Boss
                show_dialogue_message("The Citadel is sealed. Oracle says you must defeat the bosses to enter.Meet Oracle at (46,13)."); // <-- FIXED
                break;
            default:
                show_dialogue_message("You are not powerful enough! Meet Oracle at (46,13)."); // <-- FIXED
                break;
            }
            return; 
        }
        else
        {
            Player checkpointState = player;
            Combat c;
            Player &player = static_cast<Player &>(entity);
            Character *target_ptr = map.getTileAt(newx, newy)->getCharacter();
            Enemy &target = static_cast<Enemy &>(*target_ptr);
            add_log_message("Combat Triggered!");
            audio.playMusic("../data/audio/battle-bgm.mp3");
            int k = c.fight(player, target, *this, audio);
            audio.playMusic("../data/audio/sacred-garden-10377.mp3");

            if (k == 0)
            {
                bool retry = showGameOverScreen(audio);
                audio.playMusic("../data/audio/sacred-garden-10377.mp3");
                if (retry)
                {
                    newTile->getCharacter()->take_damage((-1) * (newTile->getCharacter()->get_max_health() - newTile->getCharacter()->get_health()));
                    player = checkpointState;
                    if (player.get_health() < player.get_max_health() * 0.2)
                    {
                        player.set_health(player.get_max_health() * 0.2);
                    }
                    show_dialogue_message("Fate grants you another chance...");
                    return;
                }
                else
                {
                    clear();
                    mvprintw(rows / 2, (cols - 20) / 2, "Farewell, fallen hero...");
                    refresh();
                    napms(2000); 
                    exit(0);    
                }
            }
            else if (k == 1)
            {
                add_log_message("You defeated the enemy");
                int questID = newTile->get_doQuest();
                vector<shared_ptr<Item>> *lootBox = new vector<shared_ptr<Item>>(target.getDropLoot());

                if (!lootBox->empty())
                {
                    add_log_message("Loot menu opened...");
                    runLootMenu(player, *lootBox);
                    if (!lootBox->empty())
                    {
                        map.getTileAt(newx, newy)->setCharacter(nullptr);
                        map.getTileAt(newx, newy)->setRequiredQuestCompleted(-1);
                        map.getTileAt(newx, newy)->set_doQuest(-1);
                        map.getTileAt(newx, newy)->setMapDisplayChar("◛");
                        map.getTileAt(newx, newy)->setMiniMapDisplayChar("◛");

                        map.getTileAt(newx, newy)->set_map_color_pair(5);
                        map.getTileAt(newx, newy)->set_mini_map_color_pair(5);

                        map.getTileAt(newx, newy)->setLootOnTile(lootBox);
                        map.getTileAt(newx, newy)->setIsWalkable(false);

                        add_log_message("You left some items behind.");
                    }
                    else
                    {
                        delete lootBox;
                        map.getTileAt(newx, newy)->setIsWalkable(true);
                        map.getTileAt(newx, newy)->setCharacter(nullptr);
                        map.getTileAt(newx, newy)->set_map_color_pair(6);
                        map.getTileAt(newx, newy)->set_mini_map_color_pair(6);
                        map.getTileAt(newx, newy)->setMapDisplayChar(".");
                        map.getTileAt(newx, newy)->setMiniMapDisplayChar(".");
                        map.getTileAt(newx, newy)->setRequiredQuestCompleted(-1);
                        map.getTileAt(newx, newy)->set_doQuest(-1);
                        add_log_message("You looted everything.");
                    }
                }
                else
                {
                    delete lootBox;
                    add_log_message("The enemy dropped nothing.");
                    map.getTileAt(newx, newy)->setIsWalkable(true);
                    map.getTileAt(newx, newy)->setRequiredQuestCompleted(-1);
                    map.getTileAt(newx, newy)->set_doQuest(-1);
                    map.getTileAt(entity.get_x(), entity.get_y())->setCharacter(nullptr);
                    map.getTileAt(entity.get_x(), entity.get_y())->set_map_color_pair(6);
                    map.getTileAt(entity.get_x(), entity.get_y())->set_mini_map_color_pair(6);
                    map.getTileAt(newx, newy)->setCharacter(&entity);
                    map.getTileAt(newx, newy)->setMiniMapDisplayChar("♞");
                    map.getTileAt(entity.get_x(), entity.get_y())->setMiniMapDisplayChar(".");
                    map.getTileAt(newx, newy)->setMapDisplayChar("♞");
                    map.getTileAt(entity.get_x(), entity.get_y())->setMapDisplayChar(".");
                    map.getTileAt(newx, newy)->set_map_color_pair(5);
                    map.getTileAt(newx, newy)->set_mini_map_color_pair(5);
                    entity.set_x(newx);
                    entity.set_y(newy);
                }
                if (questID != -1)
                {
                    quest[questID] = true;
                    std::vector<std::string> complete_lines;
                    switch (questID)
                    {
                    case 0:
                        complete_lines = this->hattori.complete_quest_war_chief();
                        break;
                    case 1:
                        complete_lines = this->hattori.complete_quest_orc_raider();
                        break;
                    case 2:
                        complete_lines = this->hattori.complete_quest_infernal_imp();
                        break;
                    case 3:
                        complete_lines = this->hattori.complete_quest_golem();
                        break;
                    case 4:
                        complete_lines = this->hattori.complete_quest_necromancer();
                        break;
                    case 5:
                        complete_lines = this->hattori.complete_quest_final_boss();
                        audio.playMusic("../data/audio/game_end.mp3");
                        play_cinematic_dialogue(complete_lines);
                        run_credits();
                        exit(0);
                        return;
                        break;
                    default:
                        complete_lines = this->hattori.complete_quest_final_boss();
                        break;
                    }
                    play_dialogue(complete_lines, player, map);
                }
            }
            else if (k == 2)
            {
                newTile->getCharacter()->take_damage((-1) * (newTile->getCharacter()->get_max_health() - newTile->getCharacter()->get_health()));
                add_log_message("You fled the battle!");
            }
            return;
        }
    }
    else if (map.getTileAt(newx, newy)->getBounds())
    {
        add_log_message("Do not venture outside the forest!");
        return;
    }
    else if (map.getTileAt(newx, newy)->getLootOnTile() != nullptr)
    {
        Player &player = static_cast<Player &>(entity);
        vector<shared_ptr<Item>> *lootBox = map.getTileAt(newx, newy)->getLootOnTile();
        add_log_message("Loot menu opened...");
        runLootMenu(player, *lootBox);

        if (!lootBox->empty())
        {
            add_log_message("You left some items behind.");
        }
        else
        {
            map.getTileAt(newx, newy)->setIsWalkable(true);
            delete lootBox;
            map.getTileAt(newx, newy)->setLootOnTile(nullptr);
            map.getTileAt(newx, newy)->set_map_color_pair(6);
            map.getTileAt(newx, newy)->set_mini_map_color_pair(6);
            map.getTileAt(newx, newy)->setMapDisplayChar(".");
            map.getTileAt(newx, newy)->setMiniMapDisplayChar(".");
            add_log_message("You looted everything.");
        }
        return;
    }

    else if (map.getTileAt(newx, newy)->getIsWalkable())
    {
        audio.playSFX("../data/audio/move-sfx.mp3");
        map.getTileAt(entity.get_x(), entity.get_y())->setCharacter(nullptr);
        map.getTileAt(newx, newy)->setCharacter(&entity);
        map.getTileAt(entity.get_x(), entity.get_y())->set_map_color_pair(6);
        map.getTileAt(entity.get_x(), entity.get_y())->set_mini_map_color_pair(6);
        map.getTileAt(newx, newy)->setMiniMapDisplayChar("♞");
        map.getTileAt(entity.get_x(), entity.get_y())->setMiniMapDisplayChar(".");
        map.getTileAt(newx, newy)->setMapDisplayChar("♞");
        map.getTileAt(entity.get_x(), entity.get_y())->setMapDisplayChar(".");
        map.getTileAt(newx, newy)->set_map_color_pair(5);
        map.getTileAt(newx, newy)->set_mini_map_color_pair(5);
        entity.set_x(newx);
        entity.set_y(newy);
        return;
    }
    else
    {
        add_log_message("You can't move there!");
        return;
    }
    add_log_message("Achievement Unlocked! The Void!");
}