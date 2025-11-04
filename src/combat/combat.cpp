#include "../../include/combat.hpp"
#include "../../include/enemy.h"
#include "../../extern/pdcurses/curses.h"

#include <chrono>
#include <thread>
#include <typeinfo>

using namespace std;
using namespace std::chrono;

void Combat::start() {
    clearScreen();
    mvprintw(0, 0, "=== Combat Started! ===");
    refresh();
}

void Combat::end() {
    mvprintw(0, 0, "\n=== Combat Ended! ===");
    refresh();
}

int Combat::fight(Player& p, Enemy& e) {
    start();

    // PDCurses setup for real-time input
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    auto startTime = steady_clock::now();
    auto lastPlayerNormal = startTime;
    auto lastPlayerSpecial = startTime;
    auto lastEnemyAttack = startTime;

    int enemyTurnCount = 0;

    int playerNormalInterval = static_cast<int>(p.getNormalAttackInterval() * 1000);
    int playerSpecialInterval = static_cast<int>(p.getSpecialAttackInterval() * 1000);
    int enemyAttackInterval = static_cast<int>(e.getNormalAttackInterval() * 1000);

    bool fled = false;

    while (p.isAlive() && e.isAlive()) {
        auto now = steady_clock::now();
        bool acted = false;
        int row = 0;

        int ch = getch(); // ✅ Non-blocking key input

        // === PLAYER FLEE ===
        if (ch == 'f' || ch == 'F') {
            clearScreen();
            mvprintw(0, 0, "%s flees from battle!", p.get_name().c_str());
            refresh();
            fled = true;
            break;
        }

        // === PLAYER NORMAL ATTACK (key 'l') ===
        if (ch == 'l' || ch == 'L') {
            if (duration_cast<milliseconds>(now - lastPlayerNormal).count() >= playerNormalInterval) {
                clearScreen();
                mvprintw(row++, 0, "%s performs a normal attack on %s!", 
                         p.get_name().c_str(), e.get_name().c_str());
                e.take_damage(p.getAttackPower());
                lastPlayerNormal = now;
                acted = true;
            } else {
                clearScreen();
                mvprintw(row++, 0, "Normal attack cooling down...");
                acted = true;
            }
        }

        // === PLAYER SPECIAL ATTACK (key 'r') ===
        if (ch == 'r' || ch == 'R') {
            if (duration_cast<milliseconds>(now - lastPlayerSpecial).count() >= playerSpecialInterval) {
                clearScreen();
                mvprintw(row++, 0, "%s uses a special move!", p.get_name().c_str());
                p.special_move(e);
                lastPlayerSpecial = now;
                acted = true;
            } else {
                clearScreen();
                mvprintw(row++, 0, "Special move not ready yet!");
                acted = true;
            }
        }

        // === ENEMY ATTACK (automatic) ===
        if (duration_cast<milliseconds>(now - lastEnemyAttack).count() >= enemyAttackInterval) {
            clearScreen();
            row = 0;
            enemyTurnCount++;

            bool isSpecialEnemy = 
                (typeid(e) == typeid(BoneGolem) ||
                 typeid(e) == typeid(Necromancer) ||
                 typeid(e) == typeid(LichLord));

            // Every 3rd turn → special if special enemy
            if (isSpecialEnemy && enemyTurnCount % 3 == 0) {
                mvprintw(row++, 0, "%s unleashes a special attack!", e.get_name().c_str());
                e.specialAbility(p);
            } else {
                mvprintw(row++, 0, "%s attacks!", e.get_name().c_str());
                e.attack(p);
            }

            lastEnemyAttack = now;
            acted = true;
        }

        // === STATUS DISPLAY ===
        if (acted) {
            row += 2;
            mvprintw(row++, 0, "-----------------------------------");
            mvprintw(row++, 0, "%s HP: %d / %d", p.get_name().c_str(), p.get_health(), p.get_max_health());
            mvprintw(row++, 0, "%s HP: %d / %d", e.get_name().c_str(), e.get_health(), e.get_max_health());
            mvprintw(row++, 0, "-----------------------------------");
            mvprintw(row++, 0, "[l] Normal Attack  [r] Special Attack  [f] Flee");
            refresh();
        }

        this_thread::sleep_for(milliseconds(100)); // smoother updates
    }

    // === END PHASE ===
    clearScreen();
    int result = 0;

    if (fled) {
        mvprintw(0, 0, "%s escaped successfully!", p.get_name().c_str());
        result = 2;
    } 
    else if (p.isAlive()) {
        mvprintw(0, 0, "%s wins the battle!", p.get_name().c_str());
        result = 1;
    } 
    else {
        mvprintw(0, 0, "%s has fallen in battle...", p.get_name().c_str());
        result = 0;
    }

    refresh();
    this_thread::sleep_for(milliseconds(1500));

    end();
    refresh();

    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);

    return result; // 0 = loss, 1 = win, 2 = fled
}

void Combat::clearScreen() {
    clear();
    refresh();
}
