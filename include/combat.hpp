#include "player.h"
#include "enemy.h"
#ifndef COMBAT_HPP
#define COMBAT_HPP

class Combat {
    private:
        void start() const;
        void end() const;
    public:
        bool fight(Player&, Enemy&);
        
};
#endif //COMBAT_HPP
