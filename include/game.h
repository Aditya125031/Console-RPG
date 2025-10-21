#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>
#include "player.h" 
#include "map.h"
using namespace std;

class Game
{
    private:
        std::deque<std::string> event_log;
        const size_t MAX_LOG_LINES = 5;
    public:
        void game_loop(Player& player);
        void display_welcome_message();
        void explore_forest(Player& player, Map& map);
        void add_log_message(std::string message);
        void display_dashboard(Player& player, Map& map); // New render function
        void show_full_map(Map& map);
};

#endif