#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include<cmath>
#include <limits>
#include <chrono>
#include <cstdlib> // Required for std::system
#include <conio.h>
#include"../include/tile.h"
#include"../include/map.h"
#include"../include/character.h"
#include "../include/player.h" 
#include "../include/items.h" 
#include "../include/game.h" 
#include "../include/enemy.h"
#include <windows.h>
// It's good practice to put the player creation in its own function
Player create_player() {
    std::cout << "Welcome to Text RPG!\n";
    std::cout << "Please enter your hero's name: ";
    std::string playerName;
    std::cin >> playerName;

    std::cout << "\nChoose your class:\n";
    std::cout << "1. Swordsman\n";
    std::cout << "2. Archer\n";
    std::cout << "3. Mage\n";
    int choice;
    std::cin >> choice;

    PlayerType playerType;
    switch (choice) {
        case 1: playerType = PlayerType::Swordsman; break;
        case 2: playerType = PlayerType::Archer; break;
        case 3: playerType = PlayerType::Mage; break;
        default:
            std::cout << "Invalid choice. Defaulting to Swordsman.\n";
            playerType = PlayerType::Swordsman;
            break;
    }
    return Player(playerName, playerType);
}


int main() 
{
    SetConsoleOutputCP(CP_UTF8);
    Game world;
    Player hero = create_player();
    world.game_loop(hero);

    return 0;
} // Only one '}' here!