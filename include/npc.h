#ifndef NPC_H
#define NPC_H

#include <string>
#include <vector>
#include "character.h"
class NPC:public Character
{
private:
    std::string name = "Oracle";

public:
    NPC():Character("Oracle the Sage",100000,100000) {}
    std::string getName() const 
    {
        return name;
    }
    std::vector<std::string> give_quest_war_chief();
    std::vector<std::string> give_quest_orc_raider();
    std::vector<std::string> give_quest_infernal_imp();
    std::vector<std::string> give_quest_necromancer();
    std::vector<std::string> give_quest_golem();
    std::vector<std::string> give_quest_final_boss();
    std::vector<std::string> complete_quest_war_chief();
    std::vector<std::string> complete_quest_orc_raider();
    std::vector<std::string> complete_quest_infernal_imp();
    std::vector<std::string> complete_quest_necromancer();
    std::vector<std::string> complete_quest_golem();
    std::vector<std::string> complete_quest_final_boss();
    
};
#endif