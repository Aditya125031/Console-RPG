#include<bits/stdc++.h>
#include"../include/tile.h"
#include"../include/map.h"
#include"../include/character.h"
#include "../include/player.h" 
#include "../include/items.h" 
#include "../include/game.h"

using namespace std;

void Game::game_loop(Player& player) {
    Map stage1(156,40,"data/map.txt");
    cout << "\n--- Your adventure begins! ---\n";
    
    int choice = 0;
    while (true) {
        stage1.render();
        cout << "\nWhat will you do?\n";
        cout << "  1. Use Special Move\n";
        cout << "  2. Show My Stats\n";
        cout << "  3. Quit Game\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0; // Reset choice
        }

        switch (choice) {
            case 1:
                player.special_move();
                break;
            case 2:
                player.show_details();
                break;
            case 3:
                return; // Exit the game loop
            default:
                cout << "Invalid command. Try again.\n";
                break;
        }
    }
}

void Game::display_welcome_message() {
    cout << "================================\n";
    cout << "   Welcome to the C++ ROG!    \n";
    cout << "================================\n\n";
}