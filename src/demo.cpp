#include<bits/stdc++.h>
#include"../include/tile.h"
#include"../include/map.h"
#include"../include/character.h"
#include "../include/player.h" 
#include "../include/items.h" 
#include "../include/game.h" 

using namespace std;

int main() {
    Game world;
    world.display_welcome_message();

    string player_name;
    cout << "Enter your character's name: ";
    getline(cin, player_name); 

    int choice = 0;
    PlayerType selected_type;

    while (true) {
        cout << "\nChoose your class:\n";
        cout << "  1. Swordsman\n";
        cout << "  2. Archer\n";
        cout << "  3. Mage\n";
        cout << "Enter your choice (1-3): ";
        
        cin >> choice;

        if (cin.fail()) {
            cout << "\nInvalid input. Please enter a number (1, 2, or 3).\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue; // Ask again
        }
        
        if (choice >= 1 && choice <= 3) {
            break; 
        } else {
            cout << "\nInvalid choice. Please select 1, 2, or 3.\n";
        }
    }

    switch (choice) {
        case 1:
            selected_type = PlayerType::Swordsman;
            break;
        case 2:
            selected_type = PlayerType::Archer;
            break;
        case 3:
            selected_type = PlayerType::Mage;
            break;
    }

    Player player(player_name, selected_type);
    
    cout << "\nYour character has been created!\n";
    player.show_details();

    world.game_loop(player);

    cout << "\nThank you for playing!\n";
    return 0;
}