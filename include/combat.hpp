#ifndef COMBAT_HPP
#define COMBAT_HPP

#include "character.h"
#include "enemy.h"
#include "player.h"
#include "../extern/pdcurses/curses.h"

// Enemy types
#include "goblin.h"
#include "golem.h"
#include "giant.h"
#include "final_boss.h"
#include "orc_raider.h"
#include "necromancer.h"
#include "war_chief.h"
#include "imp.h"
#include "infernalimp.h"
#include "stoneimp.h"

class Combat {
public:
    int fight(Player&, Enemy&);
    void start();
    void end();
    void clearScreen();   // declared as member
};

#endif // COMBAT_HPP