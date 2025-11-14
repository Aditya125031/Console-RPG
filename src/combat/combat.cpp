#include "../../include/combat.hpp"
#include "../../include/enemy.h"
#include "../../extern/pdcurses/curses.h"
#include"../../include/game.h"

#include <chrono>
#include <thread>
#include <typeinfo>

using namespace std;
using namespace std::chrono;

void Combat::start() {
    clearScreen();
    mvprintw(0, 0, "=== Combat Started! ===");
    refresh();
    this_thread::sleep_for(milliseconds(700));
}

void Combat::end() {
    mvprintw(2, 0, "\n=== Combat Ended! ===");
    refresh();
}

int Combat::fight(Player& p, Enemy& e,Game& world) {
    start();

    // Setup curses for non-blocking input
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    auto startTime = steady_clock::now();
    auto lastPlayerNormal = startTime;
    auto lastPlayerSpecial = startTime;

    int enemyTurnCount = 0;
    bool fled = false;

    // convert cooldown intervals to milliseconds
    int playerNormalInterval = static_cast<int>(p.getNormalAttackInterval() * 1000);
    int playerSpecialInterval = static_cast<int>(p.getSpecialAttackInterval() * 1000);

    while (p.isAlive() && e.isAlive()) {
        auto now = steady_clock::now();
        int ch = getch();
        bool acted = false;
        int row = 0;

        // === Player Flee ===
        if (ch == 'f' || ch == 'F') {
            clearScreen();
            mvprintw(0, 0, "%s flees from battle!", p.get_name().c_str());
            refresh();
            this_thread::sleep_for(milliseconds(1000));
            fled = true;
            break;
        }
        if (ch == 'i' || ch == 'I') {
            // 1. PAUSE: Switch to blocking input for the inventory menu
            nodelay(stdscr, FALSE);
            
            // 2. RUN: Call the Game's menu function
             world.runInventoryMenu(p, world);
            
            // 3. RESUME: Switch back to non-blocking for combat
            nodelay(stdscr, TRUE);
            noecho();
            curs_set(0);

            
                if (!p.isAlive()) break; // Check if player died (e.g., used bad item?)
            continue; // Continue to redraw UI
        }
        // === PLAYER NORMAL ATTACK ===
        if (ch == 'l' || ch == 'L') {
            auto sinceNormal = duration_cast<milliseconds>(now - lastPlayerNormal).count();
            if (sinceNormal >= playerNormalInterval) {
                clearScreen();
                mvprintw(row++, 0, "%s strikes %s with a normal attack!", 
                         p.get_name().c_str(), e.get_name().c_str());
                e.take_damage(p.getAttackPower());
                lastPlayerNormal = now;
                acted = true;
                refresh();
                this_thread::sleep_for(milliseconds(700));
            } else {
                clearScreen();
                mvprintw(row++, 0, "Normal attack not ready! (%.1fs left)",
                         (playerNormalInterval - sinceNormal) / 1000.0);
                refresh();
                this_thread::sleep_for(milliseconds(500));
            }
        }

        // === PLAYER SPECIAL ATTACK ===
        if (ch == 'r' || ch == 'R') {
            auto sinceSpecial = duration_cast<milliseconds>(now - lastPlayerSpecial).count();
            if (sinceSpecial >= playerSpecialInterval) {
                clearScreen();
                mvprintw(row++, 0, "%s uses a powerful SPECIAL move on %s!", 
                         p.get_name().c_str(), e.get_name().c_str());
                p.special_move(e);
                lastPlayerSpecial = now;
                acted = true;
                refresh();
                this_thread::sleep_for(milliseconds(900));
            } else {
                clearScreen();
                mvprintw(row++, 0, "Special move still recharging! (%.1fs left)",
                         (playerSpecialInterval - sinceSpecial) / 1000.0);
                refresh();
                this_thread::sleep_for(milliseconds(500));
            }
        }

        // === ENEMY COUNTERATTACK ===
        if (acted && e.isAlive()) {
            clearScreen();
            enemyTurnCount++;
            bool isSpecialEnemy =
                (typeid(e) == typeid(BoneGolem) ||
                 typeid(e) == typeid(Necromancer) ||
                 typeid(e) == typeid(LichLord));

            if (isSpecialEnemy && enemyTurnCount % 3 == 0) {
                mvprintw(row++, 0, "%s gathers power for a special strike!", e.get_name().c_str());
                refresh();
                this_thread::sleep_for(milliseconds(600));
                mvprintw(row++, 0, "%s unleashes a special attack!", e.get_name().c_str());
                e.specialAbility(p);
            } else {
                mvprintw(row++, 0, "%s counterattacks!", e.get_name().c_str());
                e.attack(p);
            }
            refresh();
            this_thread::sleep_for(milliseconds(800));
        }

        // === STATUS DISPLAY ===
        clearScreen();
        row = 0;
        mvprintw(row++, 0, "-----------------------------------");
        mvprintw(row++, 0, "%s HP: %d / %d", p.get_name().c_str(), p.get_health(), p.get_max_health());
        mvprintw(row++, 0, "%s HP: %d / %d", e.get_name().c_str(), e.get_health(), e.get_max_health());
        mvprintw(row++, 0, "-----------------------------------");

        auto normalCD = max(0, playerNormalInterval - (int)duration_cast<milliseconds>(now - lastPlayerNormal).count());
        auto specialCD = max(0, playerSpecialInterval - (int)duration_cast<milliseconds>(now - lastPlayerSpecial).count());
        mvprintw(row++, 0, "Normal ready in: %.1fs | Special ready in: %.1fs", 
                 normalCD / 1000.0, specialCD / 1000.0);
        mvprintw(row++, 0, "[l] Normal  [r] Special  [f] Flee");
        refresh();

        this_thread::sleep_for(milliseconds(100));
    }

    // === END PHASE ===
    clearScreen();
    int result = 0;

    if (fled) {
        mvprintw(0, 0, "%s fled successfully!", p.get_name().c_str());
        result = 2;
    } else if (p.isAlive()) {
        mvprintw(0, 0, "%s defeats %s!", p.get_name().c_str(), e.get_name().c_str());
        result = 1;
    } else {
        mvprintw(0, 0, "%s has fallen in battle...", p.get_name().c_str());
        result = 0;
    }

    refresh();
    this_thread::sleep_for(milliseconds(2000));
    end();

    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);

    return result;
}

void Combat::clearScreen() {
    clear();
    refresh();
}
