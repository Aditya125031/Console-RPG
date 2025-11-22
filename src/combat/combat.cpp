#include "../include/combat.hpp"
#include "../include/enemy.h"
#include "../extern/pdcurses/curses.h"
#include "../include/game.h"

#include <chrono>
#include <thread>
#include <typeinfo>

using namespace std;
using namespace std::chrono;

#include <string>
#include <thread>
#include <chrono>

void Combat::start()
{
    clearScreen(); 
    curs_set(0);

    int rows, cols;
    getmaxyx(stdscr, rows, cols); 
    int midY = rows / 2;
    int midX = cols / 2;

    std::string text = " FIGHT! ";
    std::string horizontal_border = "===================="; 
    std::string vertical_text = "||    FIGHT!    ||";      

    horizontal_border.resize(vertical_text.length(), '=');

    int startX = midX - (vertical_text.length() / 2);

    attron(COLOR_PAIR(4) | A_BOLD); 
    mvprintw(midY - 1, startX, "%s", horizontal_border.c_str());
    mvprintw(midY, startX, "%s", vertical_text.c_str());
    mvprintw(midY + 1, startX, "%s", horizontal_border.c_str());
    attroff(COLOR_PAIR(4) | A_BOLD);

    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(400)); 

    attron(COLOR_PAIR(6) | A_BOLD); 
    mvprintw(midY - 1, startX, "%s", horizontal_border.c_str());
    mvprintw(midY, startX, "%s", vertical_text.c_str());
    mvprintw(midY + 1, startX, "%s", horizontal_border.c_str());
    attroff(COLOR_PAIR(6) | A_BOLD);

    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    attron(COLOR_PAIR(4) | A_BOLD); 
    mvprintw(midY - 1, startX, "%s", horizontal_border.c_str());
    mvprintw(midY, startX, "%s", vertical_text.c_str());
    mvprintw(midY + 1, startX, "%s", horizontal_border.c_str());
    attroff(COLOR_PAIR(4) | A_BOLD);

    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 

    flushinp(); 
}

void Combat::endWin()
{
    clearScreen();
    curs_set(0);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int midY = rows / 2;
    int midX = cols / 2;

    std::string text_line = "|  Enemy Defeated!  |";
    std::string border_line = "+===================+";
    border_line.resize(text_line.length(), '=');
    border_line[0] = '+';
    border_line[border_line.length() - 1] = '+';

    int startX = midX - (text_line.length() / 2);

    attron(COLOR_PAIR(2) | A_BOLD); 
    mvprintw(midY - 1, startX, "%s", border_line.c_str());
    mvprintw(midY, startX, "%s", text_line.c_str());
    mvprintw(midY + 1, startX, "%s", border_line.c_str());
    attroff(COLOR_PAIR(2) | A_BOLD);

    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    attron(COLOR_PAIR(6) | A_BOLD); 
    mvprintw(midY - 1, startX, "%s", border_line.c_str());
    mvprintw(midY, startX, "%s", text_line.c_str());
    mvprintw(midY + 1, startX, "%s", border_line.c_str());
    attroff(COLOR_PAIR(6) | A_BOLD);

    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    attron(COLOR_PAIR(2) | A_BOLD); 
    mvprintw(midY - 1, startX, "%s", border_line.c_str());
    mvprintw(midY, startX, "%s", text_line.c_str());
    mvprintw(midY + 1, startX, "%s", border_line.c_str());
    attroff(COLOR_PAIR(2) | A_BOLD);

    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); \

    flushinp();
}

void Combat::endFlee()
{
    clearScreen();
    curs_set(0);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int midY = rows / 2;
    int midX = cols / 2;

    std::string text_line = "   You Retreated   ";
    std::string border_line = "-------------------";
    border_line.resize(text_line.length(), '-');

    int startX = midX - (text_line.length() / 2);

    attron(COLOR_PAIR(5)); 
    mvprintw(midY - 1, startX, "%s", border_line.c_str());
    mvprintw(midY, startX, "%s", text_line.c_str());
    mvprintw(midY + 1, startX, "%s", border_line.c_str());
    attroff(COLOR_PAIR(5));

    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(1200)); // Hold

    flushinp();
}

int Combat::fight(Player &p, Enemy &e, Game &world, AudioManager &audio)
{
    start();

    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    auto startTime = steady_clock::now();
    auto lastPlayerNormal = startTime;
    auto lastPlayerSpecial = startTime;
    auto lastEnemyAction = startTime;

    int enemyTurnCount = 0;
    bool fled = false;

    int playerNormalInterval = static_cast<int>(p.inventory.equippedWeapon->cool() * 1000);
    int playerSpecialInterval = static_cast<int>(p.inventory.equippedWeapon->sa_cool() * 1000);
    const int enemyIntervalMs = 2000;

    std::vector<std::string> combatLogs;

    auto addLog = [&](std::string msg)
    {
        combatLogs.push_back(msg);
        if (combatLogs.size() > 4)
        {
            combatLogs.erase(combatLogs.begin());
        }
    };

    auto drawCombatState = [&](std::string newMessage = "")
    {
        if (!newMessage.empty())
            addLog(newMessage);

        erase();

        int maxW = COLS;
        int maxH = LINES;
        int leftCenter = maxW / 4;
        int rightCenter = (maxW / 4) * 3;
        int msgRow = maxH / 2;

        // === PLAYER ===
        // ... (no changes to player stats)
        mvprintw(2, leftCenter - 10, "=== PLAYER ===");
        mvprintw(3, leftCenter - 10, "%s", p.get_name().c_str());

        float pH_pct = (float)p.get_health() / (float)p.get_max_health();
        int pH_bars = (int)(pH_pct * 20);
        int pColor = (pH_pct > 0.30f) ? 2 : 4;

        mvprintw(4, leftCenter - 10, "HP: ");
        attron(COLOR_PAIR(pColor));
        addch('[');
        for (int i = 0; i < 20; i++)
        {
            if (i < pH_bars)
                addch('|');
            else
                addch(' ');
        }
        addch(']');
        attroff(COLOR_PAIR(pColor));
        printw(" %d/%d", p.get_health(), p.get_max_health());

        auto now = steady_clock::now();
        auto sinceNorm = duration_cast<milliseconds>(now - lastPlayerNormal).count();
        auto sinceSpec = duration_cast<milliseconds>(now - lastPlayerSpecial).count();
        double nCD = max(0.0, (playerNormalInterval - sinceNorm) / 1000.0);
        double sCD = max(0.0, (playerSpecialInterval - sinceSpec) / 1000.0);

        mvprintw(6, leftCenter - 10, "Normal CD:  %.1fs", nCD);
        mvprintw(7, leftCenter - 10, "Special CD: %.1fs", sCD);

        // === ENEMY ===
        // ... (no changes to enemy stats)
        attron(COLOR_PAIR(4));
        mvprintw(2, rightCenter - 10, "=== ENEMY ===");
        mvprintw(3, rightCenter - 10, "%s", e.get_name().c_str());
        mvprintw(4, rightCenter - 10, "HP: [");
        attroff(COLOR_PAIR(4));

        float eH_pct = (float)e.get_health() / (float)e.get_max_health();
        int eH_bars = (int)(eH_pct * 20);

        for (int i = 0; i < 20; i++)
        {
            if (i < eH_bars)
                addch('|');
            else
                addch(' ');
        }

        attron(COLOR_PAIR(4));
        addch(']');
        attroff(COLOR_PAIR(4));
        printw(" %d/%d", e.get_health(), e.get_max_health());

        int startRow = msgRow - 2;
        int lineLen = 40;
        int startCol = (maxW - lineLen) / 2;

        mvprintw(startRow - 1, startCol, "----------------------------------------");

        for (size_t i = 0; i < combatLogs.size(); ++i)
        {
            std::string txt = combatLogs[i];
            int txtLen = txt.length();
            int txtCol = (maxW - txtLen) / 2;

            if (i == combatLogs.size() - 1)
            {
                attron(A_BOLD);
                mvprintw(startRow + i, txtCol, "%s", txt.c_str());
                attroff(A_BOLD);
            }
            else
            {
                mvprintw(startRow + i, txtCol, "%s", txt.c_str());
            }
        }

        mvprintw(startRow + 4, startCol, "----------------------------------------");

        mvprintw(maxH - 5, 2, "[ CONTROLS ]");
        mvprintw(maxH - 4, 2, "[L-CLICK] / [J] Normal Attack");
        mvprintw(maxH - 3, 2, "[R-CLICK] / [L] Special Attack");
        mvprintw(maxH - 2, 2, "[SPACE]   Flee Battle");
        mvprintw(maxH - 1, 2, "[I]       Inventory");
        // --- END MODIFICATION ---

        refresh();
    };

    while (p.isAlive() && e.isAlive())
    {
        auto now = steady_clock::now();
        int ch = getch();
        MEVENT event;
        if (ch == ' ')
        {
            drawCombatState(p.get_name() + " retreats!");
            this_thread::sleep_for(milliseconds(1000));
            flushinp();
            fled = true;
            break;
        }

        if (ch == 'i' || ch == 'I')
        {
            nodelay(stdscr, FALSE);
            world.runInventoryMenu(p, world);
            nodelay(stdscr, TRUE);
            noecho();
            curs_set(0);
            mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
            if (!p.isAlive())
                break;
            continue;
        }

        if (ch == 'j' || ch == 'J')
        {
            auto sinceNormal = duration_cast<milliseconds>(now - lastPlayerNormal).count();
            if (sinceNormal >= playerNormalInterval)
            {
                if (p.get_type_string() == "Swordsman")
                {
                    audio.playSFX("../data/audio/sword-normal.mp3");
                }
                else if (p.get_type_string() == "Archer")
                {
                    audio.playSFX("../data/audio/archer-normal.mp3");
                }
                else
                {
                    audio.playSFX("../data/audio/mage-normal.mp3");
                }
                e.take_damage(p.getAttackPower());
                lastPlayerNormal = now;

                char msg[100];
                snprintf(msg, sizeof(msg), "%s strikes %s!", p.get_name().c_str(), e.get_name().c_str());
                drawCombatState(std::string(msg));
            }
            else
            {
                char msg[100];
                double left = (playerNormalInterval - sinceNormal) / 1000.0;
                snprintf(msg, sizeof(msg), "Normal not ready! (%.1fs)", left);
                drawCombatState(std::string(msg));
            }
        }

        if (ch == 'l' || ch == 'L')
        {
            if (p.inventory.equippedWeapon->special)
            {
                auto sinceSpecial = duration_cast<milliseconds>(now - lastPlayerSpecial).count();
                if (sinceSpecial >= playerSpecialInterval)
                {
                    char msg1[100];
                    snprintf(msg1, sizeof(msg1), "%s accumulates POWER!", p.get_name().c_str());
                    drawCombatState(std::string(msg1));
                    this_thread::sleep_for(milliseconds(1000));

                    if (p.get_type_string() == "Swordsman")
                    {
                        audio.playSFX("../data/audio/sword-special.mp3");
                    }
                    else if (p.get_type_string() == "Archer")
                    {
                        audio.playSFX("../data/audio/archer-special.mp3");
                    }
                    else
                    {
                        audio.playSFX("../data/audio/mage-special.mp3");
                    }

                    p.special_move(e);
                    lastPlayerSpecial = now;

                    char msg[100];
                    snprintf(msg, sizeof(msg), "%s uses SPECIAL!", p.get_name().c_str());
                    drawCombatState(std::string(msg));
                }
                else
                {
                    char msg[100];
                    double left = (playerSpecialInterval - sinceSpecial) / 1000.0;
                    snprintf(msg, sizeof(msg), "Special recharging! (%.1fs)", left);
                    drawCombatState(std::string(msg));
                }
            }
            else
            {
                char msg[100];
                snprintf(msg, sizeof(msg), "No Special Attack!");
                drawCombatState(std::string(msg));
            }
        }

        if (ch == KEY_MOUSE)
        {
            if (nc_getmouse(&event) == OK)
            {

                if (event.bstate & BUTTON1_CLICKED)
                {
                    auto sinceNormal = duration_cast<milliseconds>(now - lastPlayerNormal).count();
                    if (sinceNormal >= playerNormalInterval)
                    {
                        if (p.get_type_string() == "Swordsman")
                        {
                            audio.playSFX("../data/audio/sword-normal.mp3");
                        }
                        else if (p.get_type_string() == "Archer")
                        {
                            audio.playSFX("../data/audio/archer-normal.mp3");
                        }
                        else
                        {
                            audio.playSFX("../data/audio/mage-normal.mp3");
                        }
                        e.take_damage(p.getAttackPower());
                        lastPlayerNormal = now;

                        char msg[100];
                        snprintf(msg, sizeof(msg), "%s strikes %s!", p.get_name().c_str(), e.get_name().c_str());
                        drawCombatState(std::string(msg));
                    }
                    else
                    {
                        char msg[100];
                        double left = (playerNormalInterval - sinceNormal) / 1000.0;
                        snprintf(msg, sizeof(msg), "Normal not ready! (%.1fs)", left);
                        drawCombatState(std::string(msg));
                    }
                }
                else if (event.bstate & BUTTON3_CLICKED)
                {
                    if (p.inventory.equippedWeapon->special)
                    {
                        auto sinceSpecial = duration_cast<milliseconds>(now - lastPlayerSpecial).count();
                        if (sinceSpecial >= playerSpecialInterval)
                        {
                            char msg1[100];
                            snprintf(msg1, sizeof(msg1), "%s accumulates POWER!", p.get_name().c_str());
                            drawCombatState(std::string(msg1));
                            this_thread::sleep_for(milliseconds(1000));

                            if (p.get_type_string() == "Swordsman")
                            {
                                audio.playSFX("../data/audio/sword-special.mp3");
                            }
                            else if (p.get_type_string() == "Archer")
                            {
                                audio.playSFX("../data/audio/archer-special.mp3");
                            }
                            else
                            {
                                audio.playSFX("../data/audio/mage-special.mp3");
                            }

                            p.special_move(e);
                            lastPlayerSpecial = now;

                            char msg[100];
                            snprintf(msg, sizeof(msg), "%s uses SPECIAL!", p.get_name().c_str());
                            drawCombatState(std::string(msg));
                        }
                        else
                        {
                            char msg[100];
                            double left = (playerSpecialInterval - sinceSpecial) / 1000.0;
                            snprintf(msg, sizeof(msg), "Special recharging! (%.1fs)", left);
                            drawCombatState(std::string(msg));
                        }
                    }
                    else
                    {
                        char msg[100];
                        snprintf(msg, sizeof(msg), "No Special Attack!");
                        drawCombatState(std::string(msg));
                    }
                }
            }
        }
        auto sinceEnemy = duration_cast<milliseconds>(now - lastEnemyAction).count();
        if (sinceEnemy >= enemyIntervalMs && e.isAlive())
        {
            // ... (no changes to enemy logic)
            enemyTurnCount++;
            bool isSpecialEnemy =
                (typeid(e) == typeid(BoneGolem) ||
                 typeid(e) == typeid(Necromancer) ||
                 typeid(e) == typeid(LichLord));

            if (isSpecialEnemy && enemyTurnCount % 3 == 0)
            {
                drawCombatState(e.get_name() + " gathers power...");
                this_thread::sleep_for(milliseconds(1000));

                e.specialAbility(p);

                drawCombatState(e.get_name() + " unleashes SPECIAL!");
            }
            else
            {
                static int normalCounter = 0;
                e.attack(p);
                normalCounter++;

                char msg[100];
                snprintf(msg, sizeof(msg), "%s counterattacks!", e.get_name().c_str());
                drawCombatState(std::string(msg));

                if (normalCounter >= 2 && isSpecialEnemy)
                {
                    normalCounter = 0;
                }
            }
            lastEnemyAction = now;
        }

        drawCombatState("");
    }
    int result = 0;
    std::string endMsg = "";

    if (fled)
    {
        result = 2;
        drawCombatState(endMsg);
        this_thread::sleep_for(milliseconds(1000));
        flushinp();
        endFlee();
    }
    else if (p.isAlive())
    {
        // ... (no changes to end phase)
        endMsg = p.get_name() + " defeats " + e.get_name() + "!";
        drawCombatState(endMsg);
        this_thread::sleep_for(milliseconds(1000));
        flushinp();
        result = 1;
        endWin();
    }
    else
    {
        // ... (no changes to end phase)
        endMsg = p.get_name() + " has fallen in battle...";
        drawCombatState(endMsg);
        this_thread::sleep_for(milliseconds(2000));
        flushinp();
        result = 0;
    }

    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);

    return result;
}
void Combat::clearScreen()
{
    clear();
    refresh();
}
