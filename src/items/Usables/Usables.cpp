#include "../include/items.h"
#include "../include/character.h"
#include "../include/player.h"
#include <bits/stdc++.h>
#include "../include/game.h"

using namespace std;


int Usables::get_usable_health()
{
    return usable_health;
}

int Usables::get_usable_mana()
{
    return usable_mana;
}

void Usables::potion_use(Player& player, Game& world)
{
    bool is_health_potion = get_usable_health() > 0;
    bool is_mana_potion = get_usable_mana() > 0;
    
    world.add_log_message(get_item_name() + " used successfully!");
    
    if (is_health_potion) 
    {
        int old_health = player.get_health();
        int old_max_health = player.get_max_health();
        
        player.add_health(get_usable_health());
        
        int health_restored = player.get_health() - old_health;
        
        world.add_log_message("Health: " + to_string(old_health) + " -> " + 
                             to_string(player.get_health()) + "/" + 
                             to_string(player.get_max_health()));
        
        if (health_restored > 0) 
        {
            world.add_log_message("Health restored: +" + to_string(health_restored));
        }
        
        world.add_log_message("HP: " + to_string(player.get_health()) + 
                             "/" + to_string(player.get_max_health()));
    }
    
    if (is_mana_potion) 
    {
        int old_mana = player.get_mana();
        int old_max_mana = player.get_max_mana();
        
        player.add_mana(get_usable_mana());
        
        int mana_restored = player.get_mana() - old_mana;
        
        world.add_log_message("Mana: " + to_string(old_mana) + " -> " + 
                             to_string(player.get_mana()) + "/" + 
                             to_string(player.get_max_mana()));
        
        if (mana_restored > 0) 
        {
            world.add_log_message("Mana restored: +" + to_string(mana_restored));
        }
        
        world.add_log_message("MP: " + to_string(player.get_mana()) + 
                             "/" + to_string(player.get_max_mana()));
    }
}