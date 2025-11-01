#pragma once
#include <bits/stdc++.h>
#include "../include/character.h"
#include "../include/player.h"
#include "../include/game.h"
using namespace std;

class Item
{
    protected:
        string item_name="";
        int extra_health=0;
        int extra_attack=0;
        int extra_mana=0;
        double  cooldown=0.0;
        bool equipped=false;
        std::chrono::steady_clock::time_point last_use;


    public:
        void sword_stats()
        {
            cout<<"Weapon Name : "<<item_name<<endl;
            cout<<"attack : "<<extra_attack<<endl;
            cout<<"extra_mana : "<<extra_mana<<endl;
        }
        void armor_stats()
        {
            cout<<"Armor Name : "<<item_name<<endl;
            cout<<"Health : "<<extra_health<<endl;
            cout<<"Mana : "<<extra_mana<<endl;
        }
        void potion_stats()
        {
            cout<<"Potion Name : "<<item_name<<endl;
            cout<<"Health : "<<extra_health<<endl;
            cout<<"Mana : "<<extra_mana<<endl;
        }

        Item() : last_use(std::chrono::steady_clock::time_point()) {}
        string get_item_name();
        int get_extra_health();
        int get_extra_attack();
        int get_extra_mana();
        bool can_use();
        double get_cooldown();
        void equip();
        void unequip();
        bool is_equipped() const;
        void update_last_use();
        virtual void apply_effects(Player& player, Game& world);
        virtual void remove_effects(Player& player, Game& world);

        static void use_potion(Item& potion, Player& player, Game& world);
};

class Shinketsu_Sword : public Item
{
    public:
        Shinketsu_Sword()
        {
            item_name = "Shinketsu_Sword";
            extra_attack = 25;
            cooldown = 1.0;
        }
};

class Great_Sword : public Item
{
    public:
        Great_Sword()
        {
            item_name = "Great_Sword";
            extra_attack = 10;
            cooldown = 2.5;
        }
};

class Legendary_Sword : public Item
{
    public:
        Legendary_Sword()
        {
            item_name = "Legendary_Sword";
            extra_attack = 40;
            extra_mana = 10;
            cooldown = 1.7;
        }
};

class Orb_of_Avarice : public Item
{
    public:
        Orb_of_Avarice()
        {
            item_name = "Orb_of_Avarice";
            extra_attack = 10;
            extra_mana = 50;
            cooldown=2.0;
        }
};

class Silent_Death : public Item
{
    public:
        Silent_Death()
        {
            item_name = "Silent_Death";
            extra_attack=25;
            extra_mana=10;
            cooldown=1.3;
        }
};

class Void_Embrace : public Item
{
    public:
        Void_Embrace()
        {
            item_name="Void_Embrace";
            extra_attack=50;
            extra_mana=20;
            cooldown=2.0;
        }
};

class Elder_Wand : public Item
{
    public:
        Elder_Wand()
        {
            item_name = "Elder Wand";
            extra_attack = 7;
            extra_mana = 30;
            cooldown=1.5;
        }
};

class Leather_Armor : public Item
{
    public:
        Leather_Armor()
        {
            item_name = "Leather_Armor";
            extra_health = 30;
        }
};

class Iron_Armor : public Item
{
    public:
        Iron_Armor()
        {
            item_name = "Iron_Armor";
            extra_health = 55;
        }
};

class Dragon_Armor : public Item
{
    public:
        Dragon_Armor()
        {
            item_name = "Dragon_Armor";
            extra_health = 100;
        }
};

class Void_Cloak : public Item
{
    public:
        Void_Cloak()
        {
            item_name = "Mage_Robes";
            extra_health = 20;
            extra_mana = 35;
        }
};

class Aether_Robe : public Item
{
    public:
        Aether_Robe()
        {
            item_name = "Aether_Robe";
            extra_health = 35;
            extra_mana = 60;
        }
};

class Health_Potion : public Item
{
    public:
        Health_Potion()
        {
            item_name = "Health_Potion";
            extra_health = 30;
            cooldown=10.0;
        }
};

class Mana_Potion : public Item
{
    public:
        Mana_Potion()
        {
            item_name = "Mana_Potion";
            extra_mana = 30;
            cooldown=10.0;
        }
};