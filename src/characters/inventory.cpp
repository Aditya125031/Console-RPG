#include "../include/inventory.hpp"
#include<iostream>
using namespace std;
 void Inventory:: pickup(string a,int x)
 {
   int cap=hash[a];
    if(itemq.find(a)!=itemq.end())
    {
        int t1=itemq[a];
        if(t1==cap)
        cout<<"Maximum limit reached"<<endl;
        else if(sub1.find(a)!=sub1.end())
        cout<<"Cannot pick more than one sword"<<endl;
        else if(sub2.find(a)!=sub2.end())
        cout<<"Cannot pick more than one armour"<<endl;
        else if(cap-t1>x)
        itemq[a]=t1+x;
        else itemq[a]=cap;
    }
    else if(t==c)
    cout<<"Bag is filled"<<endl;
    else{
      if(cap<x)
      itemq[a]=cap;
      else itemq[a]=x;
      t++;
    }
 }
/* map Inventory::equip()
 {

 }*/
void Inventory:: drop(string a){
   int t1=itemq[a];
    cout<<"select quantity"<<endl;
    int x;
    cin>>x;
    if(x>=t1){
    cout<<"Dropped all "<<a<<endl;
    auto it=itemq.find(a);
    itemq.erase(it);}
    else{
      cout<<"Dropped "<<x<<" "<<a<<endl;
      itemq[a]=t1-x;
    }
}
void Inventory::display(){
  for (const auto& item:itemq)
  {
    cout<<item.first<<"  -  "<<item.second<<"          ";
  }
}