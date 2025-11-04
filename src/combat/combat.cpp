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

    auto startTime = steady_clock::now();
    auto lastPlayerNormal = startTime;
    auto lastPlayerSpecial = startTime;
    auto lastEnemyAttack = startTime;

    int enemyTurnCount = 0;

    // Convert seconds → milliseconds
    int playerNormalInterval = static_cast<int>(p.getNormalAttackInterval() * 1000);
    int playerSpecialInterval = static_cast<int>(p.getSpecialAttackInterval() * 1000);
    int enemyAttackInterval = static_cast<int>(e.getNormalAttackInterval() * 1000);

    while (p.isAlive() && e.isAlive()) {
        auto now = steady_clock::now();
        bool acted = false;
        int row = 0; // Used for mvprintw vertical layout

        // === PLAYER NORMAL ATTACK ===
        if (duration_cast<milliseconds>(now - lastPlayerNormal).count() >= playerNormalInterval) {
            clearScreen();
            mvprintw(row++, 0, "%s performs a normal attack on %s!", 
                     p.get_name().c_str(), e.get_name().c_str());
            e.take_damage(p.getAttackPower());
            lastPlayerNormal = now;
            acted = true;
        }

        // === PLAYER SPECIAL ATTACK ===
        if (duration_cast<milliseconds>(now - lastPlayerSpecial).count() >= playerSpecialInterval) {
            clearScreen();
            mvprintw(row++, 0, "%s uses a special move!", p.get_name().c_str());
            p.special_move(e);
            lastPlayerSpecial = now;
            acted = true;
        }

        // === ENEMY ATTACK ===
        if (duration_cast<milliseconds>(now - lastEnemyAttack).count() >= enemyAttackInterval) {
            clearScreen();
            row = 0;
            enemyTurnCount++;

            bool canUseSpecial = false;

            // ✅ Only Golem, Necromancer, and Final Boss use special attack
            if (typeid(e) == typeid(BoneGolem) ||
                typeid(e) == typeid(Necromancer) ||
                typeid(e) == typeid(LichLord)) {
                canUseSpecial = true;
            }

            if (canUseSpecial && enemyTurnCount % 2 == 0) {
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
            refresh();
        }

        this_thread::sleep_for(milliseconds(600));
    }

    // === END PHASE ===
    clearScreen();
    if (p.isAlive()) {
        mvprintw(0, 0, "%s wins the battle!", p.get_name().c_str());
    } else {
        mvprintw(0, 0, "%s wins the battle!", e.get_name().c_str());
    }
    refresh();
    this_thread::sleep_for(milliseconds(1500));

    end();
    refresh();

    return p.isAlive() ? 1 : 0;
}

void Combat::clearScreen() {
    clear();
    refresh();
}