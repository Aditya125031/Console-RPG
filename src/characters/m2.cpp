#include "../include/inventory.hpp"
#include<iostream>
#include<string>
#include<vector>
#include<map>
using namespace std;
void display(vector<string>loot,vector<int>q)
{
    cout<<"Loot crate"<<endl;
    int i=0;
    while(loot[i]!=""){
    cout<<loot[i]<<":"<<q[i]<<endl;
    i++;}
}
void remove(vector<string>loot,vector<int>q,int index,int t,int l)
{
    auto it=loot.begin()+index;
    auto f=q.begin()+index;
    if(l==0)
    {
        loot.erase(it);
        loot.push_back("");
        q.erase(f);
        q.push_back(0);
    }
    else {
        q[index]=l;
    }
}
void main()
{
    vector<string>loot{20};
    vector<int>q{20};
    loot.push_back("Healing Potion");
    loot.push_back("Leather Armour");
    loot.push_back("Legendary Armour");
    loot.push_back("Mana Potion");
    loot.push_back("Iron Sword");
    q.push_back(3);
    q.push_back(1);
    q.push_back(1);
    q.push_back(21);
    q.push_back(1);
    
    Inventory inventory;
    char x;
    cin>>x;
    while(x!='x')
    {
        switch(x)
        {
            case 'p':
                cout<<"select the item to pick up"<<endl<<"If you are done,press 0 "<<endl;
                int y;
                cin>>y;
                while(y!=0)
                {
                    if(y>loot.size()||y<0||loot[y-1]=="")
                    cout<<"Invalid Position"<<endl;
                    else{
                        int t;
                        cout<<"select quantity"<<endl;
                        cin>>t;
                        if(t>0&&t<=q[y-1]){
                    int l= inventory.pickup(loot[y-1],t);
                        remove(loot,q,y-1,t,l);
                    }
                    else if(t>q[y-1])
                    {
                        int l=inventory.pickup(loot[y-1],q[y-1]);
                        remove(loot,q,y-1,q[y-1],l);
                    }
                    else cout<<"Not a quantity"<<endl;
                }
                display(loot,q);
                inventory.display();
                cin>>y;
                }
                break;
            case 'o':
                int y;
                cout<<"select the item to drop"<<endl<<"If you are done,press 0 "<<endl;
                while(y>0){
                inventory.drop(y);
                cin>>y;}
                inventory.display();
                display(loot,q);
                break;

            case 'i':
            inventory.unequiparmour();
            break;

            case 'j':
            inventory.unequipsword();
            break;

            case 'e':
                int i=0;
                while(loot[i]!="")
                {
                    int l=inventory.pickup(loot[i],q[i]);
                    remove(loot,q,i,q[i],l);
                    if(l!=0)
                    i++;

                }
                break;
        }
        cin>>x;
    }
}