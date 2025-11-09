#ifndef NPC_H
#define NPC_H
#include <string>
class NPC
{
private:
    std::string name = "hattori";

public:
    std::string getName() const 
    {
        return name;
    }void give_quest_war_chief();
    void give_quest_orc_raider();
    void give_quest_infernal_imp();
    void give_quest_necromancer();
    void give_quest_golem();
    void complete_quest_war_chief();
    void complete_quest_orc_raider();
    void complete_quest_infernal_imp();
    void complete_quest_necromancer();
    void complete_quest_golem();
};
#endif