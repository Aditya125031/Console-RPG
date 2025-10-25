#ifndef INVENTORY_HPP
#define INVENTORY_HPP
#include<iostream>
#include <map>
#include<set>
using namespace std;
class Inventory
{
    public:
    int c=20;
    int t=0;
     map<string, int> hash;
     map<string,int> itemq;
     set<string> sub1={"Leather Armour","Iron Armour","Legendary Armour"};
     set<string> sub2={"Sword","Katana","Legendary Sword"};
    Inventory()
    {
        hash["Healing Potion"]=10;
        hash["Mana Potion"]=10;
        hash["Leather Armour"]=1;
        hash["Iron Armour"]=1;
        hash["Legendary Armour"]=1;
        hash["Sword"]=1;
        hash["Katana"]=1;
        hash["Legendary Sword"]=1;
    }
    void pickup(string,int);
    void drop(string);
  //  map equip();
    void display();
};
#endif