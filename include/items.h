#pragma once
#include <bits/stdc++.h>
#include "../include/character.h"
using namespace std;
class Player;
class Game;
class Item
{
    protected:
        string item_name="";
        string description="";
        int extra_health=0;
        int extra_attack=0;
        int extra_mana=0;
        double  cooldown=0.0;
        bool equipped=false;
        int code=0;
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
            description = "A blade that drinks light and bleeds shadows, forged in the heart of a dying star.";
            item_name = "Shinketsu_Sword";
            extra_attack = 25;
            cooldown = 1.0;
            code=1;
        }
};

class Omenvein : public Item
{
    public:
        Omenvein()
        {
            description = "Its draw is the first sign of the end , a sword that foretells doom with every swing.";
            item_name = "Omenvein";
            extra_attack = 10;
            cooldown = 2.5;
            code=2;
        }
};

class God_Slayer : public Item
{
    public:
        God_Slayer()
        {
            description = "Forged from a silent god's dying breath, this sword hums with ancient power and untold legends.";
            item_name = "Legendary Sword";
            extra_attack = 40;
            extra_mana = 10;
            cooldown = 1.7;
            code=3;
        }
};

class Orb_of_Avarice : public Item
{
    public:
        Orb_of_Avarice()
        {
            description = "A pulsating orb that feeds on greed, amplifying the power of those who dare to wield it.";
            item_name = "Orb of Avarice";
            extra_attack = 10;
            extra_mana = 50;
            cooldown=2.0;
            code=4;
        }
};

class Silent_Death : public Item
{
    public:
        Silent_Death()
        {
            description = "A Bow that strikes without a sound, delivering swift and deadly justice from the shadows.";
            item_name = "Silent_Death";
            extra_attack=25;
            extra_mana=10;
            cooldown=1.3;
            code=5;
        }
};

class Void_Embrace : public Item
{
    public:
        Void_Embrace()
        {
            description = "A bow crafted from the essence of the void, its arrows seem to disappear into nothingness before striking.";
            item_name="Void_Embrace";
            extra_attack=50;
            extra_mana=20;
            cooldown=2.0;
            code=6;
        }
};

class Elder_Wand : public Item
{
    public:
        Elder_Wand()
        {
            description = "Crafted from the wood of the ancient Elder tree and imbued with powerful magic.";
            item_name = "Elder Wand";
            extra_attack = 7;
            extra_mana = 30;
            cooldown=1.5;
            code=7;
        }
};

class Leather_Armor : public Item
{
    public:
        Leather_Armor()
        {
            item_name = "Leather_Armor";
            extra_health = 30;
            code=8;
        }
};

class Iron_Armor : public Item
{
    public:
        Iron_Armor()
        {
            item_name = "Iron_Armor";
            extra_health = 55;
            code=9;
        }
};

class Dragon_Armor : public Item
{
    public:
        Dragon_Armor()
        {
            description = "Forged from the scales of ancient dragons, this armor offers unparalleled protection and a fiery aura.";
            item_name = "Dragon_Armor";
            extra_health = 100;
            extra_mana = 20;
            code=10;
        }
};

class Void_Cloak : public Item
{
    public:
        Void_Cloak()
        {
            description = "Woven from the fabric of nothingness, grants its wearer an aura of mystery and protection.";
            item_name = "Void Cloak";
            extra_health = 20;
            extra_mana = 45;
            code=11;
        }
};

class Aether_Robe : public Item
{
    public:
        Aether_Robe()
        {
            description = "A robe imbued with the essence of the Aether, enhancing both vitality and magical prowess.";
            item_name = "Aether_Robe";
            extra_health = 35;
            extra_mana = 70;
            code=12;
        }
};

class Health_Potion : public Item
{
    public:
        Health_Potion()
        {
            description = "A red potion that restores health when consumed.";
            item_name = "Health_Potion";
            extra_health = 30;
            cooldown=10.0;
            code=13;
        }
};

class Mana_Potion : public Item
{
    public:
        Mana_Potion()
        {
            description = "A blue potion that restores mana when consumed.";
            item_name = "Mana_Potion";
            extra_mana = 30;
            cooldown=10.0;
            code=14;
        }
};