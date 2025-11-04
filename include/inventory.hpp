#ifndef INVENTORY_HPP
#define INVENTORY_HPP
#include<iostream>
#include <map>
#include<set>
#include<vector>
#include<string>
using namespace std;
class Inventory
{
    public:
    const int c=20;
    int t=0;
     map<string, int> hash;
     vector<string> itemq;
     vector<int>quantity;
     vector<string>eq;
     set<string> sub1={"Leather Armour","Iron Armour","Legendary Armour"};
     set<string> sub2={"Sword","Katana","Legendary Sword"};
    Inventory()
    {
        itemq.resize(21,"");
        quantity.resize(21,0);
        eq.resize(2,"");
        hash["Healing Potion"]=10;
        hash["Mana Potion"]=10;
        hash["Leather Armour"]=1;
        hash["Iron Armour"]=1;
        hash["Legendary Armour"]=1;
        hash["Sword"]=1;
        hash["Katana"]=1;
        hash["Legendary Sword"]=1;
    }
    int pickup(string,int);
    void drop(int);
    int find(string);
    vector<string> equip(int);
    void display();
    void equiparmour(string);
    void equipsword(string);
    void remove(string);
    void use_from_inventory(int);
    bool find_sword();
    void unequipsword();
    void unequiparmour();
    void displayequip();
    bool find_armour();
};
#endif