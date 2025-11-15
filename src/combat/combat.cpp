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

    // Setup curses
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    auto startTime = steady_clock::now();
    auto lastPlayerNormal = startTime;
    auto lastPlayerSpecial = startTime;
    auto lastEnemyAction = startTime;

    int enemyTurnCount = 0;
    bool fled = false;

    // Cooldowns
    int playerNormalInterval = static_cast<int>(p.getNormalAttackInterval() * 1000);
    int playerSpecialInterval = static_cast<int>(p.getSpecialAttackInterval() * 2000);
    const int enemyIntervalMs = 2000; 

    // --- HELPER: Draw the UI ---
    // We define this lambda to keep the drawing logic consistent 
    // regardless of whether we are idling or attacking.
    auto drawCombatState = [&](std::string centralMsg) {
        erase(); // Clear buffer

        int maxW = COLS;
        int maxH = LINES;
        int leftCenter = maxW / 4;
        int rightCenter = (maxW / 4) * 3;
        int msgRow = maxH / 2; 

        // 1. Draw Player Stats (Left)
        mvprintw(2, leftCenter - 10, "=== PLAYER ===");
        mvprintw(3, leftCenter - 10, "%s", p.get_name().c_str());
        
        // Health Bar Math
        float pH_pct = (float)p.get_health() / (float)p.get_max_health();
        int pH_bars = (int)(pH_pct * 20); // 20 char bar
        mvprintw(4, leftCenter - 10, "HP: [");
        for(int i=0; i<20; i++) {
            if(i < pH_bars) addch('|'); else addch(' ');
        }
        printw("] %d/%d", p.get_health(), p.get_max_health());

        // Cooldown timers calculations (re-calculated here for display)
        auto now = steady_clock::now();
        auto sinceNorm = duration_cast<milliseconds>(now - lastPlayerNormal).count();
        auto sinceSpec = duration_cast<milliseconds>(now - lastPlayerSpecial).count();
        double nCD = max(0.0, (playerNormalInterval - sinceNorm) / 1000.0);
        double sCD = max(0.0, (playerSpecialInterval - sinceSpec) / 1000.0);

        mvprintw(6, leftCenter - 10, "Normal CD:  %.1fs", nCD);
        mvprintw(7, leftCenter - 10, "Special CD: %.1fs", sCD);

        // 2. Draw Enemy Stats (Right)
        mvprintw(2, rightCenter - 10, "=== ENEMY ===");
        mvprintw(3, rightCenter - 10, "%s", e.get_name().c_str());

        float eH_pct = (float)e.get_health() / (float)e.get_max_health();
        int eH_bars = (int)(eH_pct * 20);
        mvprintw(4, rightCenter - 10, "HP: [");
        for(int i=0; i<20; i++) {
            if(i < eH_bars) addch('|'); else addch(' ');
        }
        printw("] %d/%d", e.get_health(), e.get_max_health());

        // 3. Draw Central Message
        if (!centralMsg.empty()) {
            int msgLen = centralMsg.length();
            int startCol = (maxW - msgLen) / 2;
            // Draw a box or lines around the message for emphasis
            mvprintw(msgRow - 2, startCol, std::string(msgLen, '-').c_str());
            mvprintw(msgRow - 1, startCol, "%s", centralMsg.c_str());
            mvprintw(msgRow, startCol, std::string(msgLen, '-').c_str());
        }

        // 4. Draw Menu (Bottom Left)
        mvprintw(maxH - 4, 2, "[ CONTROLS ]");
        mvprintw(maxH - 3, 2, "[L] Normal Attack");
        mvprintw(maxH - 2, 2, "[R] Special Attack");
        mvprintw(maxH - 1, 2, "[F] Flee Battle");

        refresh();
    };

    while (p.isAlive() && e.isAlive()) {
        auto now = steady_clock::now();
        int ch = getch();
        
        // === PLAYER FLEE ===
        if (ch == 'f' || ch == 'F') {
            drawCombatState(p.get_name() + " flees from battle!");
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
                e.take_damage(p.getAttackPower());
                lastPlayerNormal = now;
                
                // Update UI with message
                char msg[100];
                snprintf(msg, sizeof(msg), "%s strikes %s (Normal)!", p.get_name().c_str(), e.get_name().c_str());
                drawCombatState(std::string(msg));
                
                this_thread::sleep_for(milliseconds(700));
            } else {
                char msg[100];
                double left = (playerNormalInterval - sinceNormal) / 1000.0;
                snprintf(msg, sizeof(msg), "Normal attack not ready! (%.1fs)", left);
                drawCombatState(std::string(msg));
                this_thread::sleep_for(milliseconds(300));
            }
        }

        // === PLAYER SPECIAL ATTACK ===
        if (ch == 'r' || ch == 'R') {
            auto sinceSpecial = duration_cast<milliseconds>(now - lastPlayerSpecial).count();
            if (sinceSpecial >= playerSpecialInterval) {
                p.special_move(e);
                lastPlayerSpecial = now;

                char msg[100];
                snprintf(msg, sizeof(msg), "%s uses SPECIAL move on %s!", p.get_name().c_str(), e.get_name().c_str());
                drawCombatState(std::string(msg));

                this_thread::sleep_for(milliseconds(900));
            } else {
                char msg[100];
                double left = (playerSpecialInterval - sinceSpecial) / 1000.0;
                snprintf(msg, sizeof(msg), "Special move recharging! (%.1fs)", left);
                drawCombatState(std::string(msg));
                this_thread::sleep_for(milliseconds(300));
            }
        }

        // === ENEMY AUTONOMOUS ACTION ===
        auto sinceEnemy = duration_cast<milliseconds>(now - lastEnemyAction).count();
        if (sinceEnemy >= enemyIntervalMs && e.isAlive()) {
            enemyTurnCount++;
            bool isSpecialEnemy =
                (typeid(e) == typeid(BoneGolem) ||
                 typeid(e) == typeid(Necromancer) ||
                 typeid(e) == typeid(LichLord));

            if (isSpecialEnemy && enemyTurnCount % 3 == 0) {
                drawCombatState(e.get_name() + " gathers power...");
                this_thread::sleep_for(milliseconds(600));
                
                e.specialAbility(p);
                
                drawCombatState(e.get_name() + " unleashes SPECIAL!");
            } else {
                static int normalCounter = 0;
                e.attack(p);
                normalCounter++;
                
                char msg[100];
                snprintf(msg, sizeof(msg), "%s counterattacks!", e.get_name().c_str());
                drawCombatState(std::string(msg));

                if (normalCounter >= 2 && isSpecialEnemy) {
                    normalCounter = 0;
                }
            }
            lastEnemyAction = now;
            this_thread::sleep_for(milliseconds(800));
        }

        // === IDLE STATE UPDATE ===
        // If no buttons pressed and no enemy action, we still redraw 
        // to update the cooldown timers in real-time.
        drawCombatState(""); // No message, just HUD update
        this_thread::sleep_for(milliseconds(100));
    }

    // === END PHASE ===
    int result = 0;
    std::string endMsg;

    if (fled) {
        endMsg = p.get_name() + " fled successfully!";
        result = 2;
    } else if (p.isAlive()) {
        endMsg = p.get_name() + " defeats " + e.get_name() + "!";
        result = 1;
    } else {
        endMsg = p.get_name() + " has fallen in battle...";
        result = 0;
    }

    drawCombatState(endMsg);
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
