#ifndef GAME_H
#define GAME_H
#define SDL_MAIN_HANDLED // <--- ADD THIS LINE FIRST
#include "../include/audiomanager.h"
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include<cmath>
#include <limits>
#include <chrono>
#include <cstdlib> // Required for std::system
#include "player.h" 
#include "map.h"
#include "NPC.h"
#include "inventory.hpp"

//class AudioManager; // Forward declaration

using namespace std;
struct DisplayItem {
    string displayName; // "Old Sword (Equipped)"
    string itemID;      // "EQUIPPED_WEAPON" or "Small Health Potion"
    string type;        // "WEAPON", "ARMOR", "POTION"
    string description;
};
class Game
{
    private:
        std::deque<std::string> event_log;
        const size_t MAX_LOG_LINES = 10;
        std::string current_dialogue_message = "";
        std::vector<std::string> current_dialogue_lines;
        std::vector<std::string> wrap_text(const std::string& text, int max_width);
        std::chrono::steady_clock::time_point lastHpRegenTime;
        std::chrono::steady_clock::time_point lastManaRegenTime;
    public:
        void game_loop(Player& player, AudioManager& audio);
        void display_welcome_message();
        void explore_forest(Player& player, Map& map, vector<bool>& quest, AudioManager& audio);
        void add_log_message(std::string message);
        void display_dashboard(Player& player, Map& map); // New render function
        void show_full_map(Map& map);
        void move_character(Character&, int, int, Map&, vector<bool>&, AudioManager& audio,Player& player);
        void runItemActionMenu(DisplayItem, Player&, Game&);
        void runInventoryMenu(Player&, Game&);
        void show_dialogue_message(const std::string& message); // <-- CORRECT
        void clear_dialogue_message();
        void play_dialogue(const std::vector<std::string>& lines, Player& player, Map& map);
        std::vector<DisplayItem> buildPlayerItemList(Player& player);
        NPC hattori;
        std::vector<std::shared_ptr<Item>>runLootMenu(Player& player, std::vector<std::shared_ptr<Item>>& lootBox);
        bool showGameOverScreen();
};


#endif