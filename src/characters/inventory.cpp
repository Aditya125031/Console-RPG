#include "inventory.hpp"
#include "../extern/pdcurses/curses.h"
#include<iostream>


void Inventory::displayequip()
{
    printw("Equipables\n"); 
    
    for(int i=0;i<2;i++)
    {
        if(eq[i]!="")
           
            printw("%s :1     ", eq[i].c_str()); 
        else
           
            printw("___     "); 
    }
   
    printw("\n"); 
}
int Inventory::find(std::string a) 
{
    for(int i=1;i<=t;i++)
    {
        if(itemq[i]==a)
            return i;
    }
    return -1;
}
bool Inventory::find_armour()
{
    if(eq[0]=="")
        return false;
    return true;
}
bool Inventory::find_sword()
{
    if(eq[1]=="")
        return false;
    return true;
}
int Inventory:: pickup(std::string a,int x) 
{
    int cap=hash[a];
    int index=find(a);
    if(sub1.find(a)!=sub1.end())
    {
        if(!find_armour()){
            equiparmour(a);
            return x-1;}
        else{
            printw("%s cannot be picked up as it is already equipped.\n", a.c_str());
            return x;}
    }
    else if(sub2.find(a)!=sub2.end())
    {
        if(!find_sword()){
            equipsword(a);
            return x-1;}
        else{ 
    
            printw("%s cannot be picked up as it is already equipped.\n", a.c_str());
            return x;}
    }
    else if(index!=-1)
    {
        int t1=quantity[index];
        if(t1==cap){
      
            printw("Maximum limit reached.\n");
            return x;}
        else if(cap-t1>x){
            quantity[index]=t1+x;
            return 0;}
        else{ quantity[index]=cap;
            return x-cap+t1;}
    }
    else if(t==c){
       
        printw("Your inventory has been filled\n");
        return x;}
    else{
        itemq[++t]=a;
        if(cap<x){
            quantity[t]=cap;
            return x-cap;}
        else {quantity[t]=x;
            return 0;}

    }
}
void Inventory::equiparmour(std::string a) 
{
    printw("%s is equipped.\n", a.c_str());
    eq[0]=a;
}
void Inventory::equipsword(std::string a) 
{
    printw("%s is equipped.\n", a.c_str());
    eq[1]=a;
}
void Inventory::unequiparmour()
{
    printw("Armour unequipped\n");
    eq[0]="";
}
void Inventory::unequipsword()
{
    printw("Sword unequipped\n");
    eq[1]="";
}
void Inventory::remove(std::string a)
{
    int index=find(a);
    if(quantity[index]==1){
      auto it=itemq.begin()+index;
     itemq.erase(it);
     itemq.push_back("");
     auto f=quantity.begin()+index;
     quantity.erase(f);
     quantity.push_back(0);
     t--;
    }
    else quantity[index]=quantity[index]-1;
}
void Inventory::use_from_inventory(int x)
{
    if(x>c||x<1)
        printw("Item Slot does not exist.\n");
    else if(x>t)
        
        printw("You chose an Empty Item Slot. \n");
    else{
        if(sub1.find(itemq[x])!=sub1.end()){
            equiparmour(itemq[x]);
            remove(itemq[x]);
        }
        else if(sub2.find(itemq[x])!=sub2.end()){
            equipsword(itemq[x]);
            remove(itemq[x]);}
        else {
            printw("%s used.\n", itemq[x].c_str());
            remove(itemq[x]);
        }
    }
}
void Inventory:: drop(int index){
    if(index>=c||itemq[index]=="")
    {
        printw("Invalid index\n");
        return;
    }
    
        printw("select quantity\n");
    int x;
     echo();
    nocbreak();
    scanw("%d", &x); 
     cbreak(); 
      noecho();
    int t1=quantity[index];
    if(x>=t1){
        
        printw("Dropped all %s\n", itemq[index].c_str());
        quantity[index]=1;
        remove(itemq[index]);
    }
    else{
        printw("Dropped %d %s\n", x, itemq[index].c_str());
        quantity[index]=t1-x;
    }
}
void Inventory::display()
{
    printw("Items in Inventory:\n");
    for(int i=1;i<c+1;i++)
    {
        if(itemq[i]!="")
            printw("%s :%d     ", itemq[i].c_str(), quantity[i]);
        else
            printw("___     "); 
    }
    printw("\n"); 
    displayequip();
    refresh(); 
}