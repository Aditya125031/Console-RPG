#include "../include/inventory.hpp"
<<<<<<< HEAD
#include "../extern/pdcurses/curses.h"
#include <algorithm> // For std::min

// --- FULL INCLUDES ---
// We need the full definitions in the .cpp file
#include "../include/items.h"
#include "../include/player.h"
#include "../include/game.h"
using namespace std;

void Inventory::reapplyAllEquipStats(Player& player, Game& world)
{
    // Reset player to base stats first
    player.reset_stats(); 
    
    // Apply stats from equipped items
    if (equippedWeapon) {
        equippedWeapon->weapon_apply_effects(player, world);
    }
    if (equippedArmor) {
        equippedArmor->armor_apply_effects(player, world);
    }
}

// --- Core Functions (addItem, swapWeapon, swapArmor, usePotion) ---
// (These are the same as the previous version)

bool Inventory::addItem(std::shared_ptr<Item> item, int quantity, Player& player, Game& world)
{
    shared_ptr<Weapon> weapon = dynamic_pointer_cast<Weapon>(item);
    if (weapon)
    {
        if (equippedWeapon == nullptr) {
            equippedWeapon = weapon;
            equippedWeapon->equip();
            world.add_log_message("Equipped " + weapon->get_item_name());
            reapplyAllEquipStats(player, world);
            return true;
        }
        else if (inventoryWeapon == nullptr) {
            inventoryWeapon = weapon;
            world.add_log_message("Added " + weapon->get_item_name() + " to inventory.");
            return true;
        }
        else {
            world.add_log_message("Weapon inventory is full!");
            return false;
        }
    }

    // --- 2. Try to cast to Armor ---
    shared_ptr<Armor> armor=dynamic_pointer_cast<Armor>(item);
    if (armor)
    {
        if (equippedArmor == nullptr) {
            equippedArmor = armor;
            equippedArmor->equip();
            world.add_log_message("Equipped " + armor->get_item_name());
            reapplyAllEquipStats(player, world);
            return true;
        }
        else if (inventoryArmor == nullptr) {
            inventoryArmor = armor;
            world.add_log_message("Added " + armor->get_item_name() + " to inventory.");
            return true;
        }
        else {
            world.add_log_message("Armor inventory is full!");
            return false;
        }
    }
 shared_ptr<Usables> potion = dynamic_pointer_cast<Usables>(item);
if (potion)
{
    string name = potion->get_item_name();

    // Check if this potion type is tracked in the max quantity map 'm'
    if (m.count(name) == 0) {
        world.add_log_message("Error: Potion '" + name + "' has no max stack size.");
        // We will add it anyway, but you should add it to your 'm' map
        // in the Inventory.h constructor.
    }
    
    // If this is the first time, store its prototype
    if (potionStorage.count(name) == 0) {
        potionStorage[name].itemPrototype = potion;
    }
    
    // Get current and max quantities
    // We'll default to 5 if not in the map
    int maxQuantity = m.count(name) ? m[name] : 5; 
    int currentQuantity = potionStorage[name].quantity;
    
    if (currentQuantity >= maxQuantity) {
        world.add_log_message(name + " stack is full!");
        return true; // Not an error, just can't add more
    }
    
    // Calculate how many we can actually add
    int roomLeft = maxQuantity - currentQuantity;
    int amountToAdd = std::min(quantity, roomLeft);
    int leftover = quantity - amountToAdd;
    
    // --- THIS IS THE LINE THAT FIXES THE BUG ---
    potionStorage[name].quantity += amountToAdd;
    
    world.add_log_message("Added " + to_string(amountToAdd) + " " + name + ".");
    
    if (leftover > 0) {
        world.add_log_message(name + " stack is now full. " + to_string(leftover) + " " + name + "(s) were not added.");
    }
    
    return true;
}
return false;
}

void Inventory::swapWeapon(Player& player, Game& world)
{
    if (equippedWeapon == nullptr && inventoryWeapon == nullptr) {
        world.add_log_message("No weapons to swap.");
        return;
    }
    world.add_log_message("Swapped weapons.");
    swap(equippedWeapon, inventoryWeapon);
    if (equippedWeapon) equippedWeapon->equip();
    if (inventoryWeapon) inventoryWeapon->unequip();
    reapplyAllEquipStats(player, world);
}

void Inventory::swapArmor(Player& player, Game& world)
{
    if (equippedArmor == nullptr && inventoryArmor == nullptr) {
        world.add_log_message("No armor to swap.");
        return;
    }
    world.add_log_message("Swapped armor.");
    std::swap(equippedArmor, inventoryArmor);
    if (equippedArmor) equippedArmor->equip();
    if (inventoryArmor) inventoryArmor->unequip();
    reapplyAllEquipStats(player, world);
}

void Inventory::usePotion(const std::string& potionName, Player& player, Game& world)
{
    if (potionStorage.count(potionName) == 0 || potionStorage[potionName].quantity <=0) {
        world.add_log_message("No " + potionName + " left.");
        return;
    }
    std::shared_ptr<Usables> potion = potionStorage[potionName].itemPrototype;
    potion->potion_use(player, world);
    potionStorage[potionName].quantity--;
}
void Inventory::dropEquippedWeapon(Game& world, Player& player)
{
    if (equippedWeapon) {
        world.add_log_message("Dropped " + equippedWeapon->get_item_name());
        equippedWeapon->unequip();
        equippedWeapon = nullptr; // Set the public pointer to null
        reapplyAllEquipStats(player, world); // Re-calc stats
    }
}
void Inventory::dropInventoryWeapon(Game& world)
{
    if (inventoryWeapon) {
        world.add_log_message("Dropped " + inventoryWeapon->get_item_name());
        inventoryWeapon = nullptr;
    }
}
void Inventory::dropEquippedArmor(Game& world, Player& player)
{
    if (equippedArmor) {
        world.add_log_message("Dropped " + equippedArmor->get_item_name());
        equippedArmor->unequip();
        equippedArmor = nullptr;
        reapplyAllEquipStats(player, world);
    }
}
void Inventory::dropInventoryArmor(Game& world)
{
    if (inventoryArmor) {
        world.add_log_message("Dropped " + inventoryArmor->get_item_name());
        inventoryArmor = nullptr;
    }
}

void Inventory::dropPotion(const std::string& potionName, Game& world)
{
    if (potionStorage.count(potionName) && potionStorage[potionName].quantity > 0)
    {
        potionStorage[potionName].quantity--;
        world.add_log_message("Dropped 1 " + potionName);
    }
}
std::map<std::string, int> Inventory::getUsablePotions() const
{
    std::map<std::string, int> usable;
    for (const auto& pair : potionStorage) {
        if (pair.second.quantity > 0) {
            usable[pair.first] = pair.second.quantity;
        }
    }
    return usable;
=======
#include<iostream>
using namespace std;
void Inventory::displayequip()
{
  cout<<"Equipables"<<endl;  
  for(int i=0;i<2;i++)
    {
      if(eq[i]!="")
      cout<<eq[i]<<" :"<<"1"<<"     ";
      else
      cout<<"___"<<"     "; 
    }
}
int Inventory::find(string a)
{
  for(int i=1;i<=t;i++)
  {
    if(itemq[i]==a)
    return i;
  }
   return -1;
}
int Inventory::findequip(string a)
{
    if(eq[0]==a)
    return 0;
    if(eq[1]==a)
    return 1;
    return -1;
}
 int Inventory:: pickup(string a,int x)
 {
   int cap=hash[a];
   int index=find(a);
   if(sub1.find(a)!=sub1.end())
   {
      if(findequip(a)==-1){
      equiparmour(a);
      return x-1;}
      else{
      cout<<a<<" cannot be picked up as it is already equipped."<<endl;
      return x;}
   }
   else if(sub2.find(a)!=sub2.end())
   {
          if(findequip(a)==-1){
          equipsword(a);
          return x-1;}
          else{ cout<<a<<" cannot be picked up as it is already equipped."<<endl;
            return x;}
   }
    else if(index!=-1)
    {
        int t1=quantity[index];
        if(t1==cap){
        cout<<"Maximum limit reached."<<endl;
        return x;}
        else if(cap-t1>x){
        quantity[index]=t1+x;
        return 0;}
        else{ quantity[index]=cap;
          return x-cap+t1;}
    }
    else if(t==c){
    cout<<"Your inventory has been filled"<<endl;
    return x;}
    else{
       itemq[++t]=a;
      if(cap<x){
      quantity[t]=cap;
      return x-cap;}
      else {quantity[t]=x;
        return 0;}
      t++;
    }
 }
 void Inventory::equiparmour(string a)
 {
      cout<<a<<" is equipped."<<endl;
      eq[0]=a;
 }
 void Inventory::equipsword(string a)
 {
      cout<<a<<" is equipped."<<endl;
      eq[1]=a;
 }
 void Inventory::unequiparmour()
 {
  cout<<"Armour unequipped"<<endl;
  eq[0]="";
 }
 void Inventory::unequipsword()
 {
  cout<<"Sword unequipped"<<endl;
  eq[1]="";
 }
 void Inventory::remove(string a)
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
   cout<<"Item Slot does not exist."<<endl;
  else if(x>t)
  cout<<"You chose an Empty Item Slot. "<<endl;
  else{
     if(sub1.find(itemq[x])!=sub1.end()){
     equiparmour(itemq[x]);
     remove(itemq[x]);
     }
     else if(sub2.find(itemq[x])!=sub2.end()){
     equipsword(itemq[x]);
     remove(itemq[x]);}
     else {
      cout<<itemq[x]<<" used."<<endl;
      remove(itemq[x]);
     }
 }
}
void Inventory:: drop(int index){
    cout<<"select quantity"<<endl;
    int x;
    cin>>x;
    int t1=quantity[index];
    if(x>=t1){
    cout<<"Dropped all "<<itemq[index]<<endl;
        quantity[index]=1;
        remove(itemq[index]);
    }
    else{
      cout<<"Dropped "<<x<<" "<<itemq[index]<<endl;
      quantity[index]=t1-x;
    }
}
void Inventory::display()
{
  cout<<"Items in Inventory:"<<endl;
    for(int i=1;i<c;i++)
    {
      if(itemq[i]!="")
      cout<<itemq[i]<<" :"<<quantity[i]<<"     ";
      else
      cout<<"___"<<"     "; 
    }
    displayequip();
>>>>>>> 10feb44bccdfd91418ab78a6e770e98ee231b7a7
}