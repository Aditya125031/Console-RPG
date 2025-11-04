#include "../include/items.h"
#include "../include/character.h"
#include "../include/player.h"
#include <bits/stdc++.h>
#include "../include/game.h"

string Armor::get_armor_name()
{
    return armor_name;
}

string Armor::get_armor_description()
{
    return armor_description;
}

int Armor::get_armor_health()
{
    return armor_health;
}

int Armor::get_armor_mana()
{
    return armor_mana;
}

void Armor::armor_apply_effects(Player& player, Game& world)
{
    player.modify_max_health(armor_health);
    world.add_log_message("Max Health : " + to_string(player.get_max_health()));
    world.add_log_message("Current Health : " + to_string(player.get_health()) + 
                        "/" + to_string(player.get_max_health()));
    player.modify_max_mana(armor_mana);
    world.add_log_message("Max Mana : " + to_string(player.get_max_mana()));
    world.add_log_message("Current Mana : " + to_string(player.get_mana()) + 
                        "/" + to_string(player.get_max_mana()));
}

void Armor::armor_remove_effects(Player& player, Game& world)
{
    player.modify_max_health(-armor_health);
    world.add_log_message("Max Health : " + to_string(player.get_max_health()));
    world.add_log_message("Current Health : " + to_string(player.get_health()) + 
                        "/" + to_string(player.get_max_health()));
    player.modify_max_mana(-armor_mana);
    world.add_log_message("Max Mana : " + to_string(player.get_max_mana()));
    world.add_log_message("Current Mana : " + to_string(player.get_mana()) + 
                        "/" + to_string(player.get_max_mana()));
}