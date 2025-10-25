#ifndef INVENTORY_HPP
#define INVENTORY_HPP
#include<iostream>
#include <map>
#include<set>
<<<<<<< HEAD
#include<vector>
#include<string>
=======
>>>>>>> f7cb41a (wip inventory.attempting to rebase)
using namespace std;
class Inventory
{
    public:
<<<<<<< HEAD
    const int c=20;
    int t=0;
     map<string, int> hash;
     vector<string> itemq{21};
     vector<int>quantity{21};
     vector<string>eq{2};
=======
    int c=20;
    int t=0;
     map<string, int> hash;
     map<string,int> itemq;
>>>>>>> f7cb41a (wip inventory.attempting to rebase)
     set<string> sub1={"Leather Armour","Iron Armour","Legendary Armour"};
     set<string> sub2={"Sword","Katana","Legendary Sword"};
    Inventory()
    {
<<<<<<< HEAD

=======
>>>>>>> f7cb41a (wip inventory.attempting to rebase)
        hash["Healing Potion"]=10;
        hash["Mana Potion"]=10;
        hash["Leather Armour"]=1;
        hash["Iron Armour"]=1;
        hash["Legendary Armour"]=1;
        hash["Sword"]=1;
        hash["Katana"]=1;
        hash["Legendary Sword"]=1;
    }
<<<<<<< HEAD
    int pickup(string,int);
    void drop(int);
    int find(string);
    vector<string> equip(int);
    void display();
    void equiparmour(string);
    void equipsword(string);
    void remove(string);
    void use_from_inventory(int);
    int findequip(string);
    void unequipsword();
    void unequiparmour();
    void displayequip();
=======
    void pickup(string,int);
    void drop(string);
  //  map equip();
    void display();
>>>>>>> f7cb41a (wip inventory.attempting to rebase)
};
#endif