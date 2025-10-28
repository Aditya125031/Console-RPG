#include <iostream>
#include <chrono>
#include <conio.h>
#include <windows.h>   // for Sleep()
#include "../../include/combat.hpp"
using namespace std;

bool Combat::fight(Player& p, Enemy& e) {
    p.setSpecialAttackInterval(3000); // in milliseconds
    p.setNormalAttackInterval(800);
    e.setNormalAttackInterval(700);  // enemy attacks once per second

    start(); // start of combat

    auto startTime = chrono::steady_clock::now();
    double nextPlayerAttack = 0.0;
    double nextPlayerSpecial = 0.0;
    double nextEnemyAttack = 0.0;

    while (p.isAlive() && e.isAlive()) {
        auto now = chrono::steady_clock::now();
        double currentTime = chrono::duration<double>(now - startTime).count(); // seconds

        // player input handeling
        if (_kbhit()) {
            char key = _getch();

            // press l for normal attack
            if (key == 'l' && currentTime >= nextPlayerAttack) {
                cout << "Player uses normal attack!\n";
                p.attack(e);
                nextPlayerAttack = currentTime + p.getAttackInterval() / 1000.0; // convert ms → sec
            }

            // press r for special attack
            else if (key == 'r' && currentTime >= nextPlayerSpecial) {
                if (p.get_mana() >= 10) {
                    cout << "Player uses special attack!\n";
                    p.special_move(e);
                    p.use_mana(10);
                    nextPlayerSpecial = currentTime + p.getSpecialAttackInterval() / 1000.0;
                } else {
                    cout << "Not enough mana!\n";
                }
            }
        }

        // enemy attack logic
        if (currentTime >= nextEnemyAttack) {
            cout << "Enemy attacks!\n";
            e.attack(p);
            nextEnemyAttack = currentTime + e.getAttackInterval() / 1000.0;
        }

        Sleep(50); // pauses for 50 ms
    }

    end(); // End combat message
    return p.isAlive(); // true if player wins
}

void Combat::start() const {
    cout << "Fight started!\n";
}
void Combat::end() const {
    cout << "Fight ended!\n";
}
