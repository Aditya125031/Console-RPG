#pragma once
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "character.h"
#include "player.h"
// #include "game.h"
class Game;
class Player;
using namespace std;

class Item
{
    protected:
        string item_name="";
        string item_description="";
        string item_use="";
        int code=0;

    public:
    virtual ~Item() = default;
        string get_item_name() 
        {
            return item_name;
        }
        string get_item_description()
        {
            return item_description;
        }
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
        int extra_attack=0;
        int extra_weapon_mana=0;
        double weapon_cooldown=0.0;
        std::chrono::steady_clock::time_point last_use;
        std::chrono::steady_clock::time_point sa_last_use;
        double special_cooldown=5.0;
        string special_description="This weapon does not have a special attack.";

    public:
        Weapon() : last_use(std::chrono::steady_clock::now()), 
                   sa_last_use(std::chrono::steady_clock::now()) 
        {
        }
        int special=false;
        string get_sd();
        int get_weapon_attack();
        int get_weapon_mana();
        bool can_use();
        bool can_use_special();
        void update_last_use();
        void update_sa_last_use();
        double get_cooldown();
        double get_sa_cooldown();
        void weapon_apply_effects(Player& player, Game& world);
        void weapon_remove_effects(Player& player, Game& world);
        virtual void special_attack(Player& player,Character& enemy,Game& world){};
        double cool();
        double sa_cool();
};

class Armor : public Equipables
{
    protected:
        int armor_health=0;
        int armor_mana=0;

    public:
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
            item_name = "Shinketsu Sword";
            extra_attack = 10;
            weapon_cooldown = 1.0;
            code=1;
        }
};

class Iron_Sword : public Weapon
{
    public:
        Iron_Sword()
        {
            item_description = "A basic Iron word. Reliable and sturdy, perfect for novice adventurers.";
            item_name = "Iron Sword";
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
            special_description ="Below 40% health: 2.5x damage (costs 10 health) | Above 40% health: 2.0x damage (costs 20 health) | Mana cost: 12";
        }
        
        void special_attack(Player& player, Character& enemy, Game& world) override;
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
            special_description = "Deals 2.5x damage and heals 30 health. Mana cost: 15";
        }
        
        void special_attack(Player& player, Character& enemy, Game& world) override;
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
            special=true;
            special_description="Deals 2.0x damage and heals 20 health. Mana cost: 30";
        }
        void special_attack(Player& player, Character& enemy, Game& world) override;
};

class Wooden_Bow : public Weapon
{
    public:
        Wooden_Bow()
        {
            item_description = "A simple bow made from sturdy oak, perfect for beginners learning the art of archery.";
            item_name = "Wooden Bow";
            extra_attack=4;
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
            item_name = "Silent Death";
            extra_attack=8;
            weapon_cooldown=1.3;
            code=1;
        }
};

class Eclipse_Striker : public Weapon
{
    public:
        Eclipse_Striker()
        {
            item_description = "Crafted during a total solar eclipse, its arrows blot out the very light as they fly.";
            item_name = "Eclipse Striker";
            extra_attack=15;
            extra_weapon_mana=10;
            weapon_cooldown=1.75;
            code=1;
            special=true;
            special_description="Deals 1.75x damage and heals 10 health. Mana cost: 20";
        }
        void special_attack(Player& player, Character& enemy, Game& world) override;
};

class Void_Embrace : public Weapon
{
    public:
        Void_Embrace()
        {
            item_description = "A bow crafted from the essence of the void, its arrows seem to disappear into nothingness before striking.";
            item_name="Void Embrace";
            extra_attack=20;
            extra_weapon_mana=20;
            weapon_cooldown=2.0;
            code=1;
            special=true;
            special_description="Deals void damage based on health percentage: Above 75% health: 1.5x | 35-75% health: 2.0x | Below 35% health: 2.5x (gains 15 health) | Mana cost: 25";
        }

        void special_attack(Player& player, Character& enemy, Game& world) override;
};

class Novice_Wand : public Weapon
{
    public:
        Novice_Wand()
        {
            item_description = "A basic wand for aspiring mages, channeling modest magical energies.";
            item_name = "Novice Wand";
            extra_attack = 3;
            weapon_cooldown=2.5;
            code=1;
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
            special_description="Above 70% mana: 2.0x damage | 40-70% mana: 2.5x damage | Below 40% mana: 3.0x damage (takes 15 self-damage) | Mana cost: 45";
        }
        
        void special_attack(Player& player, Character& enemy, Game& world) override;
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
            item_name = "Runeforged Armor";
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
            item_name = "Dragon Armor";
            armor_health = 100;
            armor_mana = 20;
            code=2;
        }
};

class Mystic_Veil : public Armor
{
    public:
        Mystic_Veil()
        {
            item_description = "A veil woven from mystical energies, enhancing both health and mana regeneration.";
            item_name = "Mystic Veil";
            armor_health = 15;
            armor_mana = 30;
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
            item_name = "Aether Robe";
            armor_health = 35;
            armor_mana = 70;
            code=2;
        }
};

class Phantom_Hide: public Armor
{
    public:
        Phantom_Hide()
        {
            item_description = " Armor crafted from the elusive phantom beasts, offering a balance of health and mana enhancement.";
            item_name = "Phantom Hide";
            armor_health = 30;
            armor_mana = 15;
            code = 2;
        }
};

class Lunar_Veil : public Armor
{
    public:
        Lunar_Veil()
        {
            item_description = "Blackened leather armor that drinks the light, perfect for those who hunt from the shadows with silent arrows.";
            item_name = "Lunar Veil";
            armor_health = 45;
            armor_mana = 25;
            code = 2;
        }
};

class Divine_Aegis : public Armor
{
    public:
        Divine_Aegis()
        {
            item_description = " Armor blessed by celestial forces, providing exceptional protection and mana regeneration for the wielder.";
            item_name = "Divine Aegis";
            armor_health = 55;
            armor_mana = 35;
            code = 2;
        }
};

class Health_Potion : public Usables
{
    public:
        Health_Potion()
        {
            item_description = "A red potion that restores health when consumed.";
            item_name = "Health Potion";
            usable_health = 30;
            code=3;
        }
};

class Angel_Blessings : public Usables
{
    public:
        Angel_Blessings()
        {
            item_description = "A divine potion blessed by angels, restoring a moderate amount of health when consumed.";
            item_name = "Angel's Blessings";
            usable_health = 50;
            code=3;
        }
};

class Elixir_Of_Life : public Usables
{
    public:
        Elixir_Of_Life()
        {
            item_description = "Elixir of life, healing wounds and increasing vitality.";
            item_name = "Elixir Of Life";
            usable_health = 70;
            code=3;
        }
};

class Mana_Potion : public Usables
{
    public:
        Mana_Potion()
        {
            item_description = "A blue potion that restores mana when consumed.";
            item_name = "Mana Potion";
            usable_mana = 30;
            code=3;
        }
};

class Dragon_Breath : public Usables
{
    public:
        Dragon_Breath()
        {
            item_description = "A mystical potion brewed from dragon's breath, restoring a moderate amount of mana when consumed.";
            item_name = "Dragon Breath";
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