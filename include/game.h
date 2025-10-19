#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>
#include "player.h" 
using namespace std;

class Game
{
    public:
        void game_loop(Player& player);
        void display_welcome_message();
};

#endif