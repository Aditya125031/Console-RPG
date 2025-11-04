#include "../include/items.h"
#include "../include/character.h"
#include "../include/player.h"
#include <bits/stdc++.h>
#include "../include/game.h"

using namespace std;

// string Weapon::get_weapon_name()
// {
//     return weapon_name;
// }

// string Weapon::get_weapon_description()
// {
//     return weapon_description;
// }

int Weapon::get_weapon_attack()
{
    return extra_attack;
}

int Weapon::get_weapon_mana()
{
    return extra_weapon_mana;
}

bool Weapon::can_use()
{
    auto now = std::chrono::steady_clock::now(); // used auto becuase Deduce the type of now automatically from whatever std::chrono::steady_clock::now() returns
    auto dif = std::chrono::duration_cast<std::chrono::duration<double>>(now-last_use);
    return dif.count() >= weapon_cooldown;
}

bool Weapon::can_use_special()
{
    auto now = std::chrono::steady_clock::now(); // used auto becuase Deduce the type of now automatically from whatever std::chrono::steady_clock::now() returns
    auto dif = std::chrono::duration_cast<std::chrono::duration<double>>(now-sa_last_use);
    return dif.count() >= special_cooldown;
}

void Weapon::update_last_use()
{
    last_use = std::chrono::steady_clock::now();
}

void Weapon::update_sa_last_use()
{
    sa_last_use = std::chrono::steady_clock::now();
}

double Weapon::get_cooldown()
{
    auto now = std::chrono::steady_clock::now();
    auto dif = std::chrono::duration_cast<std::chrono::duration<double>>(now-last_use);
    double remaining=dif.count()-weapon_cooldown;

    return remaining>0 ? remaining : 0;
}

void Weapon::weapon_apply_effects(Player& player, Game& world)
{
    player.modify_attack(extra_attack);
    world.add_log_message("Attack Power : " + to_string(player.get_attack_power()));
    player.modify_max_mana(extra_weapon_mana);
    world.add_log_message("Max Mana : " + to_string(player.get_max_mana()));
    world.add_log_message("Current Mana: " + to_string(player.get_mana()) + 
                        "/" + to_string(player.get_max_mana()));
}

void Weapon::weapon_remove_effects(Player& player, Game& world)
{
    player.modify_attack(-extra_attack);
    world.add_log_message("Attack Power : " + to_string(player.get_attack_power()));
    player.modify_max_mana(-extra_weapon_mana);
    world.add_log_message("Max Mana : " + to_string(player.get_max_mana()));
    world.add_log_message("Current Mana: " + to_string(player.get_mana()) + 
                        "/" + to_string(player.get_max_mana()));
}