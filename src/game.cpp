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

const unsigned int MAX_LOG_LINES = 10;
struct CreditLine {
    std::string text;
    int color_pair; 
    bool bold;
};
void Game::run_credits() {
    clear();
    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);

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
        {"PDCurses Team", 5, false},
        {"StackOverflow", 5, false},
        {"Gemini", 5, false},
        {"", 0, false},
        
        {"==================================", 1, true},
        {"THANKS FOR PLAYING!", 2, true},
        {"==================================", 1, true},
        {"", 0, false}
    };

    curs_set(0);
    nodelay(stdscr, TRUE); 

    int total_lines = credits.size();
    int start_row = term_height; 

    for (int offset = 0; offset < term_height + total_lines; ++offset) {
        clear();

        for (int i = 0; i < total_lines; ++i) {
            int row = start_row + i - offset;

            if (row >= 0 && row < term_height) {
                std::string text = credits[i].text;
                int col = (term_width - text.length()) / 2;
                if (col < 0) col = 0;

                if (credits[i].bold) attron(A_BOLD);
                if (credits[i].color_pair > 0) attron(COLOR_PAIR(credits[i].color_pair));

                mvprintw(row, col, "%s", text.c_str());

                if (credits[i].bold) attroff(A_BOLD);
                if (credits[i].color_pair > 0) attroff(COLOR_PAIR(credits[i].color_pair));
            }
        }

        refresh();

        int ch = getch();
        if (ch != ERR) break; 

        std::this_thread::sleep_for(std::chrono::milliseconds(180));
    }

    nodelay(stdscr, FALSE);
    clear();
    std::string exit_msg = "F A R E W E L L   R O S E N    H E R O.";
    mvprintw(term_height / 2, (term_width - exit_msg.length()) / 2, "%s", exit_msg.c_str());
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    std::string prompt = "[ Press any key to continue ]";
    mvprintw(rows - 2, (cols - prompt.length()) / 2, "%s", prompt.c_str());
    refresh();
    getch(); 
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
        if(getch())
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

    // 3. Add Potions
    for (const auto &pair : player.inventory.potionStorage)
    {
        if (pair.second.quantity > 0)
            itemMap.push_back({pair.first + " (x" + to_string(pair.second.quantity) + ")",
                               pair.first, "POTION", pair.second.itemPrototype->get_item_description() + "\nHeal Amount : " + to_string(pair.second.itemPrototype->get_usable_health()) + "\nRestores Mana : " + to_string(pair.second.itemPrototype->get_usable_mana()) + "\n"});
    }

    return itemMap;
}

vector<shared_ptr<Item>> Game::runLootMenu(Player &player,
                                           vector<shared_ptr<Item>> &lootBox)
{
    bool inLootMenu = true;
    Game &world = *this; 

    while (inLootMenu)
    {
        clear();
        mvprintw(0, 0, "--- LOOT BOX ---");
        int maxLootDisplay = 9; 
        for (int i = 0; i < lootBox.size() && i < maxLootDisplay; ++i)
        {
            mvprintw(i + 2, 0, " [%d] %s", i + 1, lootBox[i]->get_item_name().c_str());
        }
        if (lootBox.empty())
        {
            mvprintw(2, 0, " (Empty)");
        }

        mvprintw(0, 35, "--- YOUR INVENTORY ---");
        vector<DisplayItem> playerItems = buildPlayerItemList(player);
        for (int i = 0; i < playerItems.size() && i < 26; ++i)
        {
            mvprintw(i + 2, 35, " [%c] %s", 'a' + i, playerItems[i].displayName.c_str());
        }
        int row = max(12, (int)playerItems.size() + 4);
        mvprintw(row++, 0, "--- ACTIONS ---");
        mvprintw(row++, 0, "(l) Loot from Crate");
        mvprintw(row++, 0, "(d) Drop from Inventory");
        mvprintw(row++, 0, "(q) Quit Looting");
        mvprintw(row, 0, "Choice: ");
        refresh();

        int ch = getch();
        switch (ch)
        {
        case 'q':
        case 'Q':
            inLootMenu = false;
            break;

        case 'l':
        case 'L':
        {
            mvprintw(row + 1, 0, "Press Space to auto pick up or Select Loot item number (1-9): ");
            refresh();
            char ch_loot = getch();
            switch (ch_loot)
            {
            case ' ':{
                for(int i=0;i<lootBox.size();i++)
                {
                    bool success=player.inventory.addItem(lootBox[i],1,player,world);
                    if(success)
                    {
                        lootBox.erase(lootBox.begin()+i);
                        i--;
                    }
                }
            break;}
            default:{
            int index = ch_loot - '1'; // '1' -> 0

                if (index >= 0 && index < lootBox.size()) {
                    auto itemToTake = lootBox[index];
                    clear();
                    mvprintw(0,0,"Item Description: %s", itemToTake->get_item_description().c_str());
                    mvprintw(1,0,"Enter Y for YES");
                    mvprintw(2,0,"Press Any Other Key for NO");
                    char choice=getch();
                    if(choice=='Y'||choice=='y'){
                    bool success = player.inventory.addItem(itemToTake, 1, player, world);
                    
                    if (success) {
                        lootBox.erase(lootBox.begin() + index); // Remove from loot box
                    } else {
                        mvprintw(row + 2, 0, "Inventory is full! Press any key...");
                        getch(); // Pause
                    }
                refresh();}
                } else {
                    mvprintw(row + 2, 0, "Invalid number. Press any key...");
                    getch(); // Pause
                }
                break;
            }

        }
        break;
    }
        

        // === DROP FROM INVENTORY ===
        case 'd':
        case 'D':
        {
            mvprintw(row + 1, 0, "Drop item letter (a-z): ");
            refresh();
            int ch_drop = getch();
            int index = ch_drop - 'a';

            if (index >= 0 && index < playerItems.size())
            {
                DisplayItem itemToDrop = playerItems[index];
                clear();
                mvprintw(0, 0, "Drop %s?", itemToDrop.displayName.c_str());
                mvprintw(2, 0, "Description: %s", itemToDrop.description.c_str());
                mvprintw(6, 0, "(d) Drop");
                mvprintw(7, 0, "(c) Cancel");
                refresh();

                int ch_confirm = getch();
                if (ch_confirm == 'd' || ch_confirm == 'D')
                {
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
            }
            else
            {
                mvprintw(row + 2, 0, "Invalid letter. Press any key...");
                getch(); 
            }
            break;
        } 
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
    audio.playMusic("../data/audio/game_end.mp3");
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
        "           O N C E    A G A I N . . .             "};

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
        case ' ':
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
void Game::runItemActionMenu(DisplayItem selectedItem, Player &player, Game &world)
{
    clear();
    printw("Selected: %s\n Item Description: %s", selectedItem.displayName.c_str(), selectedItem.description.c_str());
    printw("------------------\n");

    if (selectedItem.type == "WEAPON" || selectedItem.type == "ARMOR")
    {
        printw("(e or u) Equip / Swap\n");
    }
    else if (selectedItem.type == "POTION")
    {
        printw("(e or u) Use\n");
    }

    printw("(d) Drop\n");
    printw("(c) Cancel\n");
    refresh();

    char ch = getch();
    switch (ch)
    {
    case 'e':
    case 'u': 
        if (selectedItem.type == "WEAPON")
        {
            player.inventory.swapWeapon(player, world);
        }
        else if (selectedItem.type == "ARMOR")
        {
            player.inventory.swapArmor(player, world);
        }
        else if (selectedItem.type == "POTION")
        {
            player.inventory.usePotion(selectedItem.itemID, player, world);
        }
        break;

    case 'd': 
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
        break;

    case 'c':
    default:
        break;
    }
}
void Game::runInventoryMenu(Player &player, Game &world)
{
    bool inInventory = true;
    while (inInventory)
    {
        clear();
        printw("--- INVENTORY --- (Select # or 'q' to quit)\n\n");

        vector<DisplayItem> itemMap = buildPlayerItemList(player);
        // --- Display the list ---
        if (itemMap.empty())
        {
            printw(" (Empty)\n");
        }
        else
        {
            for (int i = 0; i < itemMap.size(); ++i)
            {
                printw(" %d: %s\n", i + 1, itemMap[i].displayName.c_str());
            }
        }

        printw("\nSelect item #: ");
        refresh();

        char ch = getch();

        if (ch == 'q')
        {
            inInventory = false;
            continue;
        }

        int choice = ch - '0';

        if (choice > 0 && choice <= itemMap.size())
        {
            DisplayItem selectedItem = itemMap[choice - 1];
            runItemActionMenu(selectedItem, player, world);
        }
        else
        {
            printw("\nInvalid selection. Press any key.");
            getch();
        }
    }
    clear();
    printw("You close your bag.\n");
    refresh();
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
            move_character(player, 0, -1, map, quest, audio, player);
            break;
        case 's':
        case 'S':
            move_character(player, 0, 1, map, quest, audio, player);
            break;
        case 'a':
        case 'A':
            move_character(player, -1, 0, map, quest, audio, player);
            break;
        case 'd':
        case 'D':
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
    }

    nodelay(stdscr, FALSE);
}

void Game::game_loop(Player &player, AudioManager &audio)
{
    clear();
    vector<bool> quest(6, false);
    Map stage1(player, quest, 153, 37, "../data/map/map.txt");
    printw("\n--- You find your way to a nearby village to rest. ---\n");
    bool isGameRunning = true;
    refresh();

    while (isGameRunning)
    {
        clear();
        int row = 0;

        mvprintw(row++, 0, "\n--- Village Menu ---");
        mvprintw(row++, 0, "1. Show My Stats");
        mvprintw(row++, 0, "2. Venture into the forest");
        mvprintw(row++, 0, "3. Exit Game");
        mvprintw(row, 0, "Choice: ");
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
        case '1':
            clear();
            player.show_details();
            mvprintw(getmaxy(stdscr) - 2, 0, "Press any key to return...");
            refresh();
            getch();
            break;
        case '2':
            explore_forest(player, stage1, quest, audio);
            if (!player.isAlive())
            {
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
void Game::move_character(Character &entity, int x, int y, Map &map, vector<bool> &quest, AudioManager &audio, Player &player)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int newx = entity.get_x() + y;
    int newy = entity.get_y() + x;
    Tile *newTile = map.getTileAt(newx, newy);
    Tile *oldTile = map.getTileAt(entity.get_x(), entity.get_y());
    clear_dialogue_message();
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
            { 
                dialogue_lines = hattori_ptr->give_quest_golem();
            }
            else if (!quest[4])
            { 
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
                show_dialogue_message("A dark aura repels you. You must complete Oracle's other tasks first.Meet Oracle at (46,13).");
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
            int k = c.fight(player, target, *this);
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