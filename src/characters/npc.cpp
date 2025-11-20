// in NPC.cpp
#include "NPC.h"
#include <vector> 
#include <string>  

std::vector<std::string> NPC::give_quest_war_chief() {
    std::string prefix = getName() + ": ";
    return {
        prefix + "A brutal War Chief has taken over the northern stronghold . He's a problem that needs handling. Show him our strength."
    };
}

std::vector<std::string> NPC::give_quest_orc_raider() {
    std::string prefix = getName() + ": ";
    return {
        prefix + "Orc Raiders are plundering the trade road to the south west. Find their hidden camp in the foothills and dispense justice.",
        prefix + "It's time we cleared them out.'",
    };
}

std::vector<std::string> NPC::give_quest_infernal_imp(){
    std::string prefix = getName() + ": ";
    return {
        prefix + "I've had reports of an Infernal Imp towards north east. Vile, cunning creatures. Go... and be careful. Don't let it fool you."
    };
}

std::vector<std::string> NPC::give_quest_necromancer() {
    std::string prefix = getName() + ": ";
    return {
        prefix + "This is a dark omen. A **Necromancer** is defiling the ancient crypts to the west.",
        prefix + "This evil must be stopped before he raises an army of the dead and threatens the entire region. This is a top priority."
    };
}

std::vector<std::string> NPC::give_quest_golem() {
    std::string prefix = getName() + ": ";
    return {
        prefix + "The earth itself groans. A **Stone Golem**, a relic of a forgotten age, has awakened in the southern quarry.",
        prefix + "It is a force of pure destruction. This is not a hunt, warrior... this is a battle for survival. It must be stopped."
    };
}

std::vector<std::string> NPC::give_quest_final_boss() {
    std::string prefix = getName() + ": ";
    return {
        prefix + "You have overcome every trial. The Golem... the Necromancer... I... I scarcely believed it possible.",
        prefix + "But the source of this darkness remains. The one who pulls the strings.",
        prefix + "His name is **Malakor the Shadow Lord**, and he waits for you in the castle",
        prefix + "This is the end, warrior. This is the final battle. Go. End this nightmare. For all of us."
    };
}

std::vector<std::string> NPC::complete_quest_war_chief() {
    std::string prefix = getName() + ": ";
    return {
        prefix + "So, the War Chief is defeated. Good. You have done the village a service. Well done, warrior."
    };
}

std::vector<std::string> NPC::complete_quest_orc_raider() {
    std::string prefix = getName() + ": ";
    return {
        prefix + "The trade road is clear? Hmph. You are efficient. Take this for your trouble."
    };
}

std::vector<std::string> NPC::complete_quest_infernal_imp() {
    std::string prefix = getName() + ": ";
    return {
        prefix + "You've dealt with the Imp? Good riddance. I trust you weren't outsmarted by that little fiend."
    };
}

std::vector<std::string> NPC::complete_quest_necromancer() {
    std::string prefix = getName() + ": ";
    return {
        prefix + "The Necromancer... is gone? You have done the impossible. You have lifted a shadow that could have consumed us all. The entire region owes you a great debt."
    };
}
std::vector<std::string> NPC::complete_quest_golem() {
    std::string prefix = getName() + ": ";
    return {
        prefix + "You... stopped the Golem? To stand against such power and prevail... Your strength is truly legendary. I... I am in your debt. We are all in your debt."
    };
}
std::vector<std::string> NPC::complete_quest_final_boss() {
    std::string prefix ="";
    return {
        prefix + "...He is... gone? ...The shadows... they're... receding.",
        prefix + "You've done it. You have truly done it. The world is saved. I... I have no words left.",
        prefix + "Thank you, hero. You have earned your rest... and our eternal gratitude."
    };
}