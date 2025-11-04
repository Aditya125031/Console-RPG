#include "../include/items.h"
#include "../include/character.h"
#include "../include/player.h"
#include <bits/stdc++.h>
#include "../include/game.h"

using namespace std;

string Item::get_item_name()
{
    return item_name;
}

int Item::get_extra_health()
{
    return extra_health;
}

int Item::get_extra_attack()
{
    return extra_attack;
}

int Item::get_extra_mana()
{
    return extra_mana;
}

bool Item::can_use()
{
    auto now = std::chrono::steady_clock::now(); // used auto becuase Deduce the type of now automatically from whatever std::chrono::steady_clock::now() returns
    auto dif = std::chrono::duration_cast<std::chrono::duration<double>>(now-last_use);
    return dif.count() >= cooldown;
}

double Item::get_cooldown()
{
    auto now = std::chrono::steady_clock::now();
    auto dif = std::chrono::duration_cast<std::chrono::duration<double>>(now-last_use);
    double remaining=dif.count()-cooldown;

    return remaining>0 ? remaining : 0;
}

void Item::equip()
{
    equipped=true;
}

void Item::unequip()
{
    equipped=false;
}

bool Item::is_equipped() const
{
    return equipped;
}

void Item::update_last_use()
{
    last_use = std::chrono::steady_clock::now();
}

void Item::apply_effects(Player& player, Game& world)
{
    player.modify_max_health(extra_health);
    world.add_log_message("Current Health : " + to_string(player.get_health()));
    world.add_log_message("Max Health : " + to_string(player.get_max_health()));
    player.modify_attack(extra_attack);
    world.add_log_message("Attack Power : "+to_string(player.get_attack_power()));
    player.modify_max_mana(extra_mana);
    world.add_log_message("Current Mana : "+to_string(player.get_mana()));
    world.add_log_message("Max Mana : "+to_string(player.get_max_mana()));
}

void Item::remove_effects(Player& player, Game& world)
{

    player.modify_max_health(-extra_health);
    world.add_log_message("Current Health : " + to_string(player.get_health()));
    world.add_log_message("Max Health : " + to_string(player.get_max_health()));
    player.modify_attack(-extra_attack);
    world.add_log_message("Attack Power : "+to_string(player.get_attack_power()));
    player.modify_max_mana(-extra_mana);
    world.add_log_message("Current Mana : "+to_string(player.get_mana()));
    world.add_log_message("Max Mana : "+to_string(player.get_max_mana()));
}

void Item::use_potion(Item& potion, Player& player, Game& world)
{
    if(potion.can_use())
    {
        potion.apply_effects(player,world);
        potion.update_last_use();
        world.add_log_message(potion.get_item_name() + " used successfully!");
        world.add_log_message(potion.get_extra_mana()+" mana restored.");

    }
    else
    {
        double remaining = potion.get_cooldown();
        world.add_log_message("Potion is on cooldown. Please wait " + to_string(remaining) + " seconds.");
    }
}
