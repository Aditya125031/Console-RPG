#include <iostream>
#include <string>
#include <limits> 
#include "player.h" 

void game_loop(Player& player);
void display_welcome_message();

int main() {
    display_welcome_message();

    std::string player_name;
    std::cout << "Enter your character's name: ";
    std::getline(std::cin, player_name); 

    int choice = 0;
    PlayerType selected_type;

    while (true) {
        std::cout << "\nChoose your class:\n";
        std::cout << "  1. Swordsman\n";
        std::cout << "  2. Archer\n";
        std::cout << "  3. Mage\n";
        std::cout << "Enter your choice (1-3): ";
        
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "\nInvalid input. Please enter a number (1, 2, or 3).\n";
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            continue; // Ask again
        }
        
        if (choice >= 1 && choice <= 3) {
            break; 
        } else {
            std::cout << "\nInvalid choice. Please select 1, 2, or 3.\n";
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
    
    std::cout << "\nYour character has been created!\n";
    player.show_details();

    game_loop(player);

    std::cout << "\nThank you for playing!\n";
    return 0;
}

void game_loop(Player& player) {
    std::cout << "\n--- Your adventure begins! ---\n";
    
    int choice = 0;
    while (true) {
        std::cout << "\nWhat will you do?\n";
        std::cout << "  1. Use Special Move\n";
        std::cout << "  2. Show My Stats\n";
        std::cout << "  3. Quit Game\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                std::cout << "Invalid command. Try again.\n";
                break;
        }
    }
}

void display_welcome_message() {
    std::cout << "================================\n";
    std::cout << "   Welcome to the C++ ROG!    \n";
    std::cout << "================================\n\n";
}