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

double Weapon::get_sa_cooldown()
{
    auto now = std::chrono::steady_clock::now();
    auto dif = std::chrono::duration_cast<std::chrono::duration<double>>(now-sa_last_use);
    double remaining=dif.count()-special_cooldown;

    return remaining>0 ? remaining : 0;
}

void Weapon::weapon_apply_effects(Player& player, Game& world)
{
    int old_mana = player.get_max_mana();
    player.modify_attack(extra_attack);
    world.add_log_message("Attack Power : " + to_string(player.get_attack_power()));
    player.modify_max_mana(extra_weapon_mana);
    world.add_log_message("Max Mana :  " + to_string(old_mana) + " -> " + to_string(player.get_max_mana()));
}

void Weapon::weapon_remove_effects(Player& player, Game& world)
{
    int old_mana = player.get_max_mana();
    player.modify_attack(-extra_attack);
    world.add_log_message("Attack Power : " + to_string(player.get_attack_power()));
    player.modify_max_mana(-extra_weapon_mana);
    world.add_log_message("Max Mana :  " + to_string(old_mana) + " -> " + to_string(player.get_max_mana()));
}

void Soul_Reaper::special_attack(Player& player, Character& enemy, Game& world)
{
    if(special == true)
    {
        if(!can_use_special())
        {
            double remaining = get_sa_cooldown();
            world.add_log_message("Soul Reaper's special attack is on cooldown. " + to_string(remaining) + " seconds remaining.");
            return;
        }
        if(player.get_mana() < 10)
        {
            world.add_log_message("Not enough mana to perform Soul Reaper's special attack.");
            return;
        }

        double health_percentage = (double)player.get_health() / player.get_max_health();
        double damage_multiplier;
        
        if (health_percentage < 0.4) 
        {
            damage_multiplier = 2.75;
            world.add_log_message("Desperate times! Soul Reaper resonates with your fading vitality...");
            player.add_health(-10);
        } 
        else 
        {
            damage_multiplier = 2.0;
            world.add_log_message("Soul Reaper draws upon your life force...");
            player.add_health(-20); 
        }

        int damage = player.get_attack_power() * damage_multiplier;
        enemy.take_damage(damage);
        player.use_mana(12);

        world.add_log_message("Soul Reaper's 'SOUL HARVEST' unleashed! Dealt " + to_string(damage) + " damage to the enemy.");
        world.add_log_message("Player healed for 20 health.");
        
        update_sa_last_use();
    }
}

void God_Slayer::special_attack(Player& player, Character& enemy, Game& world)
{
    if(special==true)
    {
        if(!can_use_special())
        {
            double remaining = get_sa_cooldown();
            world.add_log_message("God Slayer's special attack is on cooldown. " + to_string(remaining) + " seconds remaining.");
            return;
        }

        if(player.get_mana()<15)
        {
            world.add_log_message("Not enough mana to perform God Slayer's special attack.");
            return;
        }

        int damage = player.get_attack_power() * 2.5;
        enemy.take_damage(damage);
        player.use_mana(15);
        player.add_health(30);

        world.add_log_message("God Slayer's 'DIVINE SLASH' unleashed! Dealt " + to_string(damage) + " damage to the enemy.");
        world.add_log_message("Player healed for 40 health.");
        
        update_sa_last_use();
    }
}

void Void_Embrace::special_attack(Player& player, Character& enemy, Game& world)
{
    if(special==true)
    {
        if(!can_use_special())
        {
            double remaining = get_sa_cooldown();
            world.add_log_message("The Void stirs impatiently... " + to_string(remaining) + " seconds until it can embrace again.");
            return;
        }

        if(player.get_mana() < 25)
        {
            world.add_log_message("The Void Embrace hungers for more void energy... 25 required to awaken its embrace.");
            return;
        }
        
        double health_percentage = (double)player.get_health() / player.get_max_health();
        double void_multiplier;
        string void_technique;
        
        if (health_percentage > 0.75) 
        {
            void_multiplier = 1.5;
            void_technique = "VOID'S WHISPER";
            world.add_log_message("The Void gently stirs... " + void_technique + " manifests!");
        } 
        else if (health_percentage > 0.35) 
        {
            void_multiplier = 2.0;
            void_technique = "ABYSSAL FLAMES";
            world.add_log_message("Desperation fuels the darkness... " + void_technique + " emerges from the depths!");
            player.add_health(5);
            world.add_log_message("The Void grants 5 additional health for your resilience.");
        }
        else 
        {
            void_multiplier = 2.5;
            void_technique = "ATOMIC VOID";
            world.add_log_message("At death's door, you unleash " + void_technique + "! The Void consumes all!");
            player.add_health(15);
            world.add_log_message("The Void essence seeps into you, you gained 15 health.");
        }

        int base_damage = player.get_attack_power();
        int total_damage = base_damage * void_multiplier;
        enemy.take_damage(total_damage);
        player.use_mana(25);

        world.add_log_message("Dealt " + to_string(total_damage) + " void damage.");
        world.add_log_message("Mana consumed: 25, Remaining: " + to_string(player.get_health()) + "/" + to_string(player.get_max_health()));

        update_sa_last_use();
    }
}

void Oblivion_Shard::special_attack(Player& player, Character& enemy, Game& world)
{
    if(special == true)
    {
        if(!can_use_special())
        {
            double remaining = get_sa_cooldown();
            world.add_log_message("The Oblivion Shard's special attack is on cooldown. " + to_string(remaining) + " seconds remaining.");
            return;
        }

        if(player.get_mana() < 35)
        {
            world.add_log_message("The Shard demands more soul-energy.");
            return;
        }

        double mana_percentage = (double)player.get_mana() / player.get_max_mana();
        double damage_multiplier;
        
        if (mana_percentage > 0.7) 
        {
            damage_multiplier = 2.0;
            world.add_log_message(" The Shard whispers... you have used BLACK FLASH!");
        } 
        else if (mana_percentage > 0.4) 
        {
            damage_multiplier = 2.5;
            world.add_log_message("The Shard screams! You haves used HOLLOW PURPLE !");
        }
        else 
        {
            damage_multiplier = 3.0;
            world.add_log_message("Depleting Mana!,You use your domain expansion INFINITE VOID!");
            player.take_damage(15);
            world.add_log_message("Low on Mana! You take 15 damage.");
        }

        int base_damage = player.get_attack_power();
        int damage = base_damage * damage_multiplier;
        
        enemy.take_damage(damage);
        player.use_mana(35);

        world.add_log_message("Oblivion deals " + to_string(damage) + " damage!");
        world.add_log_message("Mana consumed: 45, Remaining: " + to_string(player.get_mana()) + "/" + to_string(player.get_max_mana()));
        
        update_sa_last_use();
    }
}

void Orb_of_Avarice::special_attack(Player& player, Character& enemy, Game& world)
{
    if(special==true)
    {
        if(!can_use_special())
        {
            double remaining = get_sa_cooldown();
            world.add_log_message("Orb of Avarice's special attack is on cooldown. " + to_string(remaining) + " seconds remaining.");
            return;
        }

        if(player.get_mana()<30)
        {
            world.add_log_message("Not enough mana to perform Orb of Avarice's special attack.");
            return;
        }

        int damage = player.get_attack_power() * 2.0;
        enemy.take_damage(damage);
        player.use_mana(30);
        player.add_health(15);

        world.add_log_message("Orb of Avarice's 'GREED'S FURY' unleashed! Dealt " + to_string(damage) + " damage to the enemy.");
        world.add_log_message("Player restored 15 health.");
        
        update_sa_last_use();
    }
}

void Eclipse_Striker::special_attack(Player& player, Character& enemy, Game& world)
{
    if(special==true)
    {
        if(!can_use_special())
        {
            double remaining = get_sa_cooldown();
            world.add_log_message("Eclipse Striker's special attack is on cooldown. " + to_string(remaining) + " seconds remaining.");
            return;
        }

        if(player.get_mana()<20)
        {
            world.add_log_message("Not enough mana to perform Eclipse Striker's special attack.");
            return;
        }

        int damage = player.get_attack_power() * 2.0;
        enemy.take_damage(damage);
        player.use_mana(20);
        player.add_health(10);

        world.add_log_message("Eclipse Striker's 'SOLAR FLARE' unleashed! Dealt " + to_string(damage) + " damage to the enemy.");
        world.add_log_message("Player healed for 15 health.");
        
        update_sa_last_use();
    }
}