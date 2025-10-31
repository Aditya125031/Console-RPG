#include "../include/inventory.hpp"
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
}