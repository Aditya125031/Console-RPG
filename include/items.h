#pragma once
#include <bits/stdc++.h>
#include "../include/character.h"
#include "../include/player.h"
#include "../include/game.h"
using namespace std;

class Item
{
    protected:
        // bool equipped=false;
        string item_name="";
        string item_description="";
        int code=0;
        // std::chrono::steady_clock::time_point last_use;


    public:
        // void sword_stats()
        // {
        //     cout<<"Weapon Name : "<<item_name<<endl;
        //     cout<<"attack : "<<extra_attack<<endl;
        //     cout<<"extra_mana : "<<extra_mana<<endl;
        // }
        // void armor_stats()
        // {
        //     cout<<"Armor Name : "<<item_name<<endl;
        //     cout<<"Health : "<<extra_health<<endl;
        //     cout<<"Mana : "<<extra_mana<<endl;
        // }
        // void potion_stats()
        // {
        //     cout<<"Potion Name : "<<item_name<<endl;
        //     cout<<"Health : "<<extra_health<<endl;
        //     cout<<"Mana : "<<extra_mana<<endl;
        // }

        string get_item_name() 
        {
            return item_name;
        }
        string get_item_description()
        {
            return item_description;
        }
        void equip();
        void unequip();
        bool is_equipped() const;
        void virtual display();
        // virtual void apply_effects(Player& player, Game& world);
        // virtual void remove_effects(Player& player, Game& world);
};

class Equipables : public Item
{
    protected:
            bool equipped=false;
    public:
            void equip()
            {
                equipped=true;
            }

            void unequip()
            {
                equipped=false;
            }

            bool is_equipped() const
            {
                return equipped;
            }
};

class Weapon : public Equipables
{
    protected:
            // string item_name="";
            // string item_description="";
            int extra_attack=0;
            int extra_weapon_mana=0;
            double weapon_cooldown=0.0;
            std::chrono::steady_clock::time_point last_use;
            std::chrono::steady_clock::time_point sa_last_use;
            int special=false;
            double special_cooldown=5.0;

    public:
        Weapon() : last_use(std::chrono::steady_clock::now()), 
                   sa_last_use(std::chrono::steady_clock::now()) 
        {
        }

        int get_weapon_attack();
        int get_weapon_mana();
        bool can_use();
        bool can_use_special();
        void update_last_use();
        void update_sa_last_use();
        double get_cooldown();
        void weapon_apply_effects(Player& player, Game& world);
        void weapon_remove_effects(Player& player, Game& world);
        virtual void special_attack(Player& player,Character& enemy,Game& world){};
};

class Armor : public Equipables
{
    protected:
        int armor_health=0;
        int armor_mana=0;

    public:
        Armor()
        {
            // Default constructor for Armor
        }

        int get_armor_health();
        int get_armor_mana();
        void armor_apply_effects(Player& player,Game& world);
        void armor_remove_effects(Player& player,Game& world);
};

class Usables : public Item
{
    protected:
        int usable_health=0;
        int usable_mana=0;

    public:
        Usables()
        {
            // Default constructor for Usables
        }

        int get_usable_health();
        int get_usable_mana();

        void potion_use(Player& player,Game& world);
};

class Shinketsu_Sword : public Weapon
{
    public:
        Shinketsu_Sword()
        {
            item_description = "A blade that drinks light and bleeds shadows, forged in the heart of a dying star.";
            item_name = "Shinketsu_Sword";
            extra_attack = 10;
            weapon_cooldown = 1.0;
            code=1;
        }
};

class Omenvein : public Weapon
{
    public:
        Omenvein()
        {
            item_description = "Its draw is the first sign of the end , a sword that foretells doom with every swing.";
            item_name = "Omenvein";
            extra_attack = 5;
            weapon_cooldown = 2.0;
            code=1;
        }
};

class Soul_Reaper : public Weapon
{
    public:
        Soul_Reaper()
        {
            item_description = "A scythe that harvests the souls of the fallen, growing stronger as its wielder nears death.";
            item_name = "Soul Reaper";
            extra_attack = 15;
            extra_weapon_mana = 5;
            weapon_cooldown = 2.0;
            code = 1;
            special = true;
        }
        
        void special_attack(Player& player, Character& enemy, Game& world) override
        {
            if(special == true)
            {
                if(!can_use_special())
                {
                    double remaining = get_cooldown();
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
                    damage_multiplier = 2.5;
                    world.add_log_message("Desperate times! Soul Reaper resonates with your fading vitality...");
                    player.modify_health(-10);
                } 
                else 
                {
                    damage_multiplier = 2.0;
                    world.add_log_message("Soul Reaper draws upon your life force...");
                    player.modify_health(-20); 
                }

                int damage = player.get_attack_power() * damage_multiplier;
                enemy.take_damage(damage);
                player.use_mana(12);

                world.add_log_message("Soul Reaper's 'SOUL HARVEST' unleashed! Dealt " + to_string(damage) + " damage to the enemy.");
                world.add_log_message("Player healed for 20 health.");
                
                update_sa_last_use();
            }
        }
};

class God_Slayer : public Weapon
{
    public:
        God_Slayer()
        {
            item_description = "Forged from a silent god's dying breath, this sword hums with ancient power and untold legends.";
            item_name = "Legendary Sword";
            extra_attack = 20;
            extra_weapon_mana = 10;
            weapon_cooldown = 1.7;
            code=1;
            special=true;
        }
        
        void special_attack(Player& player,Character& enemy,Game& world) override
        {
            if(special==true)
            {
                if(!can_use_special())
                {
                    double remaining = get_cooldown();
                    world.add_log_message("God Slayer's special attack is on cooldown. " + to_string(remaining) + " seconds remaining.");
                    return;
                }

                if(player.get_mana()<15)
                {
                    world.add_log_message("Not enough mana to perform God Slayer's special attack.");
                    return;
                }

                int damage = player.get_attack_power() * 1.75;
                enemy.take_damage(damage);
                player.use_mana(15);
                player.modify_health(30);

                world.add_log_message("God Slayer's 'DIVINE SLASH' unleashed! Dealt " + to_string(damage) + " damage to the enemy.");
                world.add_log_message("Player healed for 40 health.");
                
                update_sa_last_use();
            }
        }
};

class Orb_of_Avarice : public Weapon
{
    public:
        Orb_of_Avarice()
        {
            item_description = "A pulsating orb that feeds on greed, amplifying the power of those who dare to wield it.";
            item_name = "Orb of Avarice";
            extra_attack = 10;
            extra_weapon_mana = 45;
            weapon_cooldown=2.0;
            code=1;
        }
};

class Silent_Death : public Weapon
{
    public:
        Silent_Death()
        {
            item_description = "A Bow that strikes without a sound, delivering swift and deadly justice from the shadows.";
            item_name = "Silent_Death";
            extra_attack=10;
            extra_weapon_mana=10;
            weapon_cooldown=1.3;
            code=1;
        }
};

class Void_Embrace : public Weapon
{
    public:
        Void_Embrace()
        {
            item_description = "A bow crafted from the essence of the void, its arrows seem to disappear into nothingness before striking.";
            item_name="Void_Embrace";
            extra_attack=25;
            extra_weapon_mana=20;
            weapon_cooldown=2.0;
            code=1;
            special=true;
        }

        void special_attack(Player& player,Character& enemy,Game& world) override
        {
            if(special==true)
            {
                if(!can_use_special())
                {
                    double remaining = get_cooldown();
                    world.add_log_message("Void Embrace's special attack is on cooldown. " + to_string(remaining) + " seconds remaining.");
                    return;
                }

                if(player.get_mana()<25)
                {
                    world.add_log_message("Not enough mana to perform Void Embrace's special attack.");
                    return;
                }

                int damage = player.get_attack_power() * 2.0; 
                enemy.take_damage(damage);
                player.use_mana(50);
                player.modify_health(30); 

                world.add_log_message("Void Embrace's special attack unleashed! Dealt " + to_string(damage) + " damage to the enemy.");
                world.add_log_message("Player healed for 20 health.");

                update_sa_last_use();
            }
        }
};

class Elder_Wand : public Weapon
{
    public:
        Elder_Wand()
        {
            item_description = "Crafted from the wood of the ancient Elder tree and imbued with powerful magic.";
            item_name = "Elder Wand";
            extra_attack = 7;
            extra_weapon_mana = 25;
            weapon_cooldown=1.5;
            code=1;
        }
};

class Oblivion_Shard : public Weapon
{
    public:
        Oblivion_Shard()
        {
            item_description = "A fragment of absolute nothingness that drinks memories and bleeds silence.";
            item_name = "Oblivion Shard";
            extra_attack = 15;
            extra_weapon_mana = 70;
            weapon_cooldown = 2.5;
            code = 1;
            special = true;
        }
        
        void special_attack(Player& player, Character& enemy, Game& world) override
        {
            if(special == true)
            {
                if(!can_use_special())
                {
                    double remaining = get_cooldown();
                    world.add_log_message("The Oblivion Shard's special attack is on cooldown. " + to_string(remaining) + " seconds remaining.");
                    return;
                }

                if(player.get_mana() < 45)
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
                player.use_mana(45);

                world.add_log_message("Oblivion deals " + to_string(damage) + " damage!");
                world.add_log_message("Mana consumed: 45, Remaining: " + to_string(player.get_mana()) + "/" + to_string(player.get_max_mana()));
                
                update_sa_last_use();
            }
        }
};

class Shadowhide_Armor : public Armor
{
    public:
        Shadowhide_Armor()
        {
            item_description = "Armor made from the hide of shadow beasts, offering both protection and stealth.";
            item_name = "Shadowhide Armor";
            armor_health = 30;
            code=2;
        }
};

class Runeforged_Armor : public Armor
{
    public:
        Runeforged_Armor()
        {
            item_description = "Armor etched with ancient runes that bolster the wearer's vitality and resilience.";
            item_name = "Runeforged_Armor";
            armor_health = 55;
            code=2;
        }
};

class Dragon_Armor : public Armor
{
    public:
        Dragon_Armor()
        {
            item_description = "Forged from the scales of ancient dragons, this armor offers unparalleled protection and a fiery aura.";
            item_name = "Dragon_Armor";
            armor_health = 100;
            armor_mana = 20;
            code=2;
        }
};

class Void_Cloak : public Armor
{
    public:
        Void_Cloak()
        {
            item_description = "Woven from the fabric of nothingness, grants its wearer an aura of mystery and protection.";
            item_name = "Void Cloak";
            armor_health = 20;
            armor_mana = 45;
            code=2;
        }
};

class Aether_Robe : public Armor
{
    public:
        Aether_Robe()
        {
            item_description = "A robe imbued with the essence of the Aether, enhancing both vitality and magical prowess.";
            item_name = "Aether_Robe";
            armor_health = 35;
            armor_mana = 70;
            code=2;
        }
};

class Small_Health_Potion : public Usables
{
    public:
        Small_Health_Potion()
        {
            item_description = "A red potion that restores health when consumed.";
            item_name = "Health_Potion";
            usable_health = 30;
            code=3;
        }
};

class Medium_Health_Potion : public Usables
{
    public:
        Medium_Health_Potion()
        {
            item_description = "A medium red potion that restores a moderate amount of health when consumed.";
            item_name = "Medium_Health_Potion";
            usable_health = 50;
            code=3;
        }
};

class Large_Health_Potion : public Usables
{
    public:
        Large_Health_Potion()
        {
            item_description = "A large red potion that restores a significant amount of health when consumed.";
            item_name = "Large_Health_Potion";
            usable_health = 70;
            code=3;
        }
};

class Small_Mana_Potion : public Usables
{
    public:
        Small_Mana_Potion()
        {
            item_description = "A blue potion that restores mana when consumed.";
            item_name = "Mana_Potion";
            usable_mana = 30;
            code=3;
        }
};

class Medium_Mana_Potion : public Usables
{
    public:
        Medium_Mana_Potion()
        {
            item_description = "A medium blue potion that restores a moderate amount of mana when consumed.";
            item_name = "Medium_Mana_Potion";
            usable_mana = 50;
            code=3;
        }
};

class Aether_Elixir : public Usables
{
    public:
        Aether_Elixir()
        {
            item_description = "Elixir infused with Aether energy, restoring a significant amount of mana when consumed.";
            item_name = "Aether Elixir";
            usable_mana = 70;
            code=3;
        }
};