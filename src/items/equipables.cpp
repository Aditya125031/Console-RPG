// #include "../include/items.h"
// #include "../include/character.h"
// #include "../include/player.h"
// #include <bits/stdc++.h>
// #include "../include/game.h"

// using namespace std;

// string Weapon::get_weapon_name()
// {
//     return weapon_name;
// }

// string Weapon::get_weapon_description()
// {
//     return weapon_description;
// }

// int Weapon::get_weapon_attack()
// {
//     return extra_attack;
// }

// int Weapon::get_weapon_mana()
// {
//     return extra_weapon_mana;
// }

// string Armor::get_armor_name()
// {
//     return armor_name;
// }

// string Armor::get_armor_description()
// {
//     return armor_description;
// }

// int Armor::get_armor_health()
// {
//     return armor_health;
// }

// int Armor::get_armor_mana()
// {
//     return armor_mana;
// }

// bool Weapon::can_use()
// {
//     auto now = std::chrono::steady_clock::now(); // used auto becuase Deduce the type of now automatically from whatever std::chrono::steady_clock::now() returns
//     auto dif = std::chrono::duration_cast<std::chrono::duration<double>>(now-last_use);
//     return dif.count() >= weapon_cooldown;
// }

// double Weapon::get_cooldown()
// {
//     auto now = std::chrono::steady_clock::now();
//     auto dif = std::chrono::duration_cast<std::chrono::duration<double>>(now-last_use);
//     double remaining=dif.count()-weapon_cooldown;

//     return remaining>0 ? remaining : 0;
// }

// void Item::equip()
// {
//     equipped=true;
// }

// void Item::unequip()
// {
//     equipped=false;
// }

// bool Item::is_equipped() const
// {
//     return equipped;
// }

// void Weapon::update_last_use()
// {
//     last_use = std::chrono::steady_clock::now();
//}

// void Item::apply_effects(Player& player, Game& world)
// {
//     player.modify_max_health(extra_health);
//     world.add_log_message("Current Health : " + to_string(player.get_health()));
//     world.add_log_message("Max Health : " + to_string(player.get_max_health()));
//     player.modify_attack(extra_attack);
//     world.add_log_message("Attack Power : "+to_string(player.get_attack_power()));
//     player.modify_max_mana(extra_mana);
//     world.add_log_message("Current Mana : "+to_string(player.get_mana()));
//     world.add_log_message("Max Mana : "+to_string(player.get_max_mana()));
// }

// void Item::remove_effects(Player& player, Game& world)
// {

//     player.modify_max_health(-extra_health);
//     world.add_log_message("Current Health : " + to_string(player.get_health()));
//     world.add_log_message("Max Health : " + to_string(player.get_max_health()));
//     player.modify_attack(-extra_attack);
//     world.add_log_message("Attack Power : "+to_string(player.get_attack_power()));
//     player.modify_max_mana(-extra_mana);
//     world.add_log_message("Current Mana : "+to_string(player.get_mana()));
//     world.add_log_message("Max Mana : "+to_string(player.get_max_mana()));
// }

// void Item::use_potion(Item& potion, Player& player, Game& world)
// {
//     if(potion.can_use())
//     {
//         potion.apply_effects(player,world);
//         potion.update_last_use();
//         world.add_log_message(potion.get_item_name() + " used successfully!");
//         world.add_log_message(potion.get_extra_mana()+" mana restored.");

//     }
//     else
//     {
//         double remaining = potion.get_cooldown();
//         world.add_log_message("Potion is on cooldown. Please wait " + to_string(remaining) + " seconds.");
//     }
// }

// void Item::health_potion_use(Item& potion, Player& player, Game& world)
// {
//     int old_health = player.get_health();
//     int old_max_health = player.get_max_health();
    
//     player.add_health(potion.get_extra_health());
    
//     int health_restored = player.get_health() - old_health;
    
//     world.add_log_message(potion.get_item_name() + " used successfully!");
    
//     if (potion.get_extra_health() > 0) 
//     {
//         world.add_log_message("Health: " + to_string(old_health) + " -> " + 
//                              to_string(player.get_health()) + "/" + 
//                              to_string(player.get_max_health()));
//         if (health_restored > 0) 
//         {
//             world.add_log_message("Health restored: +" + to_string(health_restored));
//         }
//     }
    
//     world.add_log_message("HP: " + to_string(player.get_health()) + 
//                          "/" + to_string(player.get_max_health()));
// }

// void Item::mana_potion_use(Item& potion, Player& player, Game& world)
// {
//     int old_mana = player.get_mana();
//     int old_max_mana = player.get_max_mana();
    
//     player.add_mana(potion.get_extra_mana());
    
//     int mana_restored = player.get_mana() - old_mana;
    
//     world.add_log_message(potion.get_item_name() + " used successfully!");
    
//     if (potion.get_extra_mana() > 0) 
//     {
//         world.add_log_message("Mana: " + to_string(old_mana) + " -> " + 
//                              to_string(player.get_mana()) + "/" + 
//                              to_string(player.get_max_mana()));
//         if (mana_restored > 0) 
//         {
//             world.add_log_message("Mana restored: +" + to_string(mana_restored));
//         }
//     }
    
//     world.add_log_message("MP: " + to_string(player.get_mana()) + 
//                          "/" + to_string(player.get_max_mana()));
// }

// bool Item::basic_attack(Player& player,Game& world,Character& target)
// {
//     if(!can_use())
//     {
//         double remaining = get_cooldown();
//         world.add_log_message("Attack is on cooldown. Please wait " + to_string(remaining) + " seconds.");
//         return false;
//     }

//     int damage = player.get_attack_power();
// }