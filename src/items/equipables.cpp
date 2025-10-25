#include "items.h"
#include "character.h"
#include "player.h"
#include<bits/stdc++.h>
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

bool Item::can_use(std::chrono::steady_clock::time_point last_use)
{
    auto now = std::chrono::steady_clock::now(); // used auto becuase Deduce the type of now automatically from whatever std::chrono::steady_clock::now() returns
    auto dif = std::chrono::duration_cast<std::chrono::duration<double>>(now-last_use);
    return dif.count() >= cooldown;
}

double Item::get_cooldown(std::chrono::steady_clock::time_point last_use)
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

// void Item::apply_effects(Player& player)
// {
//     player.modify_health(extra_health);
//     cout<<"Health : "<<player.get_health()<<endl;
//     cout<<"Max_Health : "<<player.get_max_health()<<endl;
//     player.modify_attack(extra_attack);
//     cout<<"Attack_Power : "<<player.get_attack_power()<<endl;
//     player.modify_mana(extra_mana);
//     cout<<"Mana : "<<player.get_mana()<<endl;
//     cout<<"Max_Mana : "<<player.get_max_mana()<<endl;
// }

// void Item::remove_effects(Player& player)
// {

//     player.modify_health(-extra_health);
//     cout<<"Current Health : "<<player.get_health()<<endl;
//     cout<<"Max Health : "<<player.get_max_health()<<endl;
//     player.modify_attack(-extra_attack);
//     cout<<"Attack Power : "<<player.get_attack_power()<<endl;
//     player.modify_mana(-extra_mana);
//     cout<<"Current Mana : "<<player.get_mana()<<endl;
//     cout<<"Max Mana : "<<player.get_max_mana()<<endl;
// }

// void Item::use_potion(Item& potion, Player& player, std::chrono::steady_clock::time_point& last_use)
// {
//     if(potion.can_use(last_use))
//     {
//         potion.apply_effects(player);
//         last_use = std::chrono::steady_clock::now();
//         cout << potion.get_item_name() << " used successfully!" << endl;
//         cout << potion.get_mana() << " mana restored." << endl;

//     }
//     else
//     {
//         double remaining = potion.get_cooldown(last_use);
//         cout << "Potion is on cooldown. Please wait " << remaining << " seconds." << endl;
//     }
// }