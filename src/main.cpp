#define SDL_MAIN_HANDLED // <--- ADD THIS LINE FIRST
#include "../include/audiomanager.h"
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>
#include <thread>
#include <chrono>
#include <cstdlib> // Required for std::system
// #include <windows.h> // Not needed with curses
#include "../extern/pdcurses/curses.h" // Include the curses header
#include "../include/tile.h"
#include "../include/map.h"
#include "../include/character.h"
#include "../include/player.h"
#include "../include/items.h"
#include "../include/game.h"
#include "../include/enemy.h"
#include "../include/inventory.hpp"

// Note: I'm assuming 'PlayerType' is an enum defined in one of your headers.

void draw_box(int y, int x, int w, int h)
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

// Helper to print centered text
void mvprintw_center(int y, const std::string &text)
{
    int x = (COLS - text.length()) / 2;
    mvprintw(y, x, "%s", text.c_str());
}

Player create_player(Game &game_world)
{
    clear();
    curs_set(0); // Show cursor for input

    int midY = LINES / 2;
    int midX = COLS / 2;

    // --- Phase 1: Name Input (Custom Loop) ---
    std::string playerName = "";
    bool nameEntered = false;

    // Enable keypad to detect resize events and special keys
    keypad(stdscr, TRUE);
    noecho(); // We will manually print characters

    while (!nameEntered)
    {
        // 1. Check for Resize
        if (is_termresized())
        {
            resize_term(0, 0);
            midY = LINES / 2;
            midX = COLS / 2;
            clear();
            // Note: We don't 'continue' here because we want to fall through
            // to the drawing logic immediately to update the UI.
        }

        // 2. Draw UI (Redraws every frame)

        // Title
        attron(COLOR_PAIR(1) | A_BOLD); // Cyan
        mvprintw_center(midY - 8, "|| C R E A T E   Y O U R   H E R O ||");
        attroff(COLOR_PAIR(1) | A_BOLD);

        // Box
        draw_box(midY - 5, midX - 25, 50, 4);
        mvprintw(midY - 4, midX - 23, "What is your hero's name?");

        // Input Field styling
        attron(COLOR_PAIR(5)); // Yellow
        mvprintw(midY - 2, midX - 23, "Name: [");
        mvprintw(midY - 2, midX + 21, "]");
        attroff(COLOR_PAIR(5));

        // Print the current Name buffer
        // (We clear the space first to prevent artifacts on backspace)
        mvprintw(midY - 2, midX - 15, "                              ");
        mvprintw(midY - 2, midX - 15, "%s", playerName.c_str());

        // Move cursor to the end of the typed name
        move(midY - 2, midX - 15 + playerName.length());

        refresh();

        // 3. Get Input
        int ch = getch();

        // Handle Resize Key (Some systems send this code)
        if (ch == KEY_RESIZE)
        {
            continue; // Loop back to top to handle is_termresized
        }

        // Handle Enter (Confirm)
        if (ch == 10 || ch == '\n' || ch == KEY_ENTER)
        {
            if (!playerName.empty())
            {
                nameEntered = true;
            }
        }
        // Handle Backspace (Delete)
        else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b')
        {
            if (!playerName.empty())
            {
                playerName.pop_back();
            }
        }
        // Handle Regular Characters (Limit length to fit box)
        else if (isprint(ch) && playerName.length() < 35)
        {
            playerName += (char)ch;
        }
    }

    // Restore cursor state
    curs_set(0);

    // --- Phase 2: Class Selection ---
    PlayerType playerType;
    int choice = 0;
    bool classSelected = false;

    const std::vector<std::string> classes = {"Swordsman", "Archer", "Mage"};
    const std::vector<std::string> descriptions = {
        "A balanced warrior of strength and defense.",
        "A master of ranged combat, striking from afar.",
        "A powerful wielder of arcane elemental magic."};

    while (!classSelected)
    {
        // --- RESIZE HANDLING (Phase 2) ---
        if (is_termresized())
        {
            resize_term(0, 0);
            midY = LINES / 2;
            midX = COLS / 2;
            clear();
            // Fall through to drawing logic
        }
        else
        {
            // Only clear if we didn't just resize (to avoid flickering)
            // Actually, for a menu loop, explicit clearing usually helps artifacts
            clear();
        }

        // Title
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw_center(midY - 10, "|| C R E A T E   Y O U R   H E R O ||");
        attroff(COLOR_PAIR(1) | A_BOLD);

        attron(COLOR_PAIR(6)); // White
        mvprintw_center(midY - 8, ("Welcome, " + playerName).c_str());
        attroff(COLOR_PAIR(6));

        mvprintw_center(midY - 6, "Select your class (Use UP/DOWN, Enter to select):");

        // Draw Class Options
        for (int i = 0; i < classes.size(); ++i)
        {
            int boxY = midY - 4 + (i * 4);
            int boxX = midX - 25;
            int boxW = 50;
            int boxH = 3;

            if (i == choice)
            {
                attron(COLOR_PAIR(5) | A_BOLD); // Yellow
                draw_box(boxY, boxX, boxW, boxH);
                mvprintw(boxY + 1, boxX + 2, ">> %s <<", classes[i].c_str());
                attroff(COLOR_PAIR(5) | A_BOLD);

                attron(COLOR_PAIR(6)); // White
                mvprintw_center(boxY + 2, descriptions[i].c_str());
                attroff(COLOR_PAIR(6));
            }
            else
            {
                attron(COLOR_PAIR(6)); // White
                draw_box(boxY, boxX, boxW, boxH);
                mvprintw(boxY + 1, boxX + 2, "%s", classes[i].c_str());
                attroff(COLOR_PAIR(6));
            }
        }
        refresh();

        int ch = getch();
        switch (ch)
        {
        case KEY_UP:
        case 'w':
            choice--;
            if (choice < 0)
                choice = classes.size() - 1;
            break;
        case KEY_DOWN:
        case 's':
            choice++;
            if (choice >= classes.size())
                choice = 0;
            break;
        case 10: // Enter key
            classSelected = true;
            break;
        }
    }

    // --- Final Selection ---
    switch (choice)
    {
    case 0:
        playerType = PlayerType::Swordsman;
        break;
    case 1:
        playerType = PlayerType::Archer;
        break;
    case 2:
        playerType = PlayerType::Mage;
        break;
    }

    // Loop for 1.5 seconds (1500ms), checking for resize every 50ms
    int totalDuration = 1500;
    int interval = 50;
    int elapsed = 0;

    // Initial Draw
    clear();
    // Recalculate mid just in case resize happened immediately before this block
    midY = LINES / 2;

    attron(COLOR_PAIR(2) | A_BOLD); // Green
    mvprintw_center(midY - 1, ("Welcome, " + playerName + " the " + classes[choice] + "!").c_str());
    mvprintw_center(midY + 1, "Your adventure begins...");
    attroff(COLOR_PAIR(2) | A_BOLD);
    refresh();

    while (elapsed < totalDuration)
    {
        
        if (is_termresized())
        {
            resize_term(0, 0);
            midY = LINES / 2;
            midX = COLS / 2; // <--- Added this
            clear();

            // Redraw at new center
            attron(COLOR_PAIR(2) | A_BOLD);
            mvprintw_center(midY - 1, ("Welcome, " + playerName + " the " + classes[choice] + "!").c_str());
            mvprintw_center(midY + 1, "Your adventure begins...");
            attroff(COLOR_PAIR(2) | A_BOLD);
            refresh();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        elapsed += interval;
    }
    flushinp();

    return Player(game_world, playerName, playerType);
}

// This function should be called once, before starting the game loop
void setup_curses()
{
    initscr();

    // --- Standard Game Terminal Setup ---
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // --- Color Setup ---
    if (has_colors())
    {
        start_color(); // Starts the color system

        // Initialize the color pairs using your defined constants
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_BLUE, COLOR_BLACK);
        init_pair(4, COLOR_RED, COLOR_BLACK);
        init_pair(5, COLOR_YELLOW, COLOR_BLACK);
        init_pair(6, COLOR_WHITE, COLOR_BLACK);
        init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(8, COLOR_WHITE, COLOR_WHITE);
        init_pair(9, COLOR_RED, COLOR_RED);
    }

    // The timeout below is optional, but often useful for game loops
    // sets the waiting period for getch() in milliseconds.
    // timeout(100);
}

void show_welcome_screen()
{
    std::vector<std::string> title_art = {
        " ........ ..   .. .......          ......   ....... ....... .......        ..    ........ ....... ...... ",
        "    ..    ..   .. ..               ..   ..  ..      ..      ..            ....      ..    ..      ..   ..",
        "    ..    ..   .. ..               ..    .. ..      ..      ..           ..  ..     ..    ..      ..    ..",
        "    ..    ....... .......          ..    .. ......  ....... .......     ..    ..    ..    ....... ..    ..",
        "    ..    ..   .. ..               ..    .. ..      ..      ..         ..........   ..    ..      ..    ..",
        "    ..    ..   .. ..               ..   ..  ..      ..      ..        ..        ..  ..    ..      ..   ..",
        "    ..    ..   .. .......          ......   ....... ..      ........ ..          .. ..    ....... ...... "};

    while (true)
    {
        if (is_termresized())
        {
            resize_term(0, 0);
            clear();
        }

        clear();
        int term_height, term_width;
        getmaxyx(stdscr, term_height, term_width);

        int art_height = title_art.size();
        int art_width = title_art[0].length();
        int art_start_y = (term_height / 2) - (art_height / 2) - 5;
        int art_start_x = (term_width / 2) - (art_width / 2);

        if (art_start_y < 0)
            art_start_y = 0;
        if (art_start_x < 0)
            art_start_x = 0;

        for (int i = 0; i < art_height; ++i)
        {
            std::string &line = title_art[i];

            for (int j = 0; j < line.length(); ++j)
            {
                char c = line[j];
                if (j < 28)
                {
                    if (c != ' ')
                    {
                        attron(COLOR_PAIR(8));
                        mvaddch(art_start_y + i, art_start_x + j, c);
                        attroff(COLOR_PAIR(8));
                    }
                }

                else
                {
                    if (c != ' ')
                    {
                        attron(COLOR_PAIR(9));

                        mvaddch(art_start_y + i, art_start_x + j, c);
                        attroff(COLOR_PAIR(9));
                    }
                }
            }
        }

        mvprintw(term_height - 2, 0, "Press Enter to start...");
        refresh();
        int input = getch();

        if (input == 10 || input == 13 || input == KEY_ENTER)
        {
            return;
        }
    }
}

int main()
{
    setup_curses();
    AudioManager audio;
    audio.init();
    Game world;
    audio.playMusic("../data/audio/sacred-garden-10377.mp3");
    show_welcome_screen();
    Game game;
    Player hero = create_player(game);
    hero.inventory.addItem(make_shared<Health_Potion>(), 3, hero, world);
    hero.inventory.addItem(make_shared<Mana_Potion>(), 2, hero, world);
    if (hero.get_type_string() == "Swordsman")
    {
        hero.inventory.addItem(make_shared<Iron_Sword>(), 1, hero, world);
    }
    else if (hero.get_type_string() == "Archer")
    {
        hero.inventory.addItem(make_shared<Wooden_Bow>(), 1, hero, world);
    }
    else
    {
        hero.inventory.addItem(make_shared<Novice_Wand>(), 1, hero, world);
    }
    world.game_loop(hero, audio);
    endwin();

    return 0;
}