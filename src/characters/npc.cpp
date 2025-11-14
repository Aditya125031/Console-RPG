#include "NPC.h"
#include <iostream>
void NPC::give_quest_war_chief() {
    std::cout << getName() << ": 'A brutal War Chief has taken over the northern stronghold (37,5). He's a problem that needs handling. Show him our strength.'" << std::endl;
}

void NPC::give_quest_orc_raider() {
    std::cout << getName() << ": 'Orc Raiders are plundering the trade road to the east (9,27) . Find their hidden camp in the foothills and dispense justice. It's time we cleared them out.'" << std::endl;
}

void NPC::give_quest_infernal_imp(){
    std::cout << getName() << ": 'I've had reports of an Infernal Imp near the old volcano (71,17). Vile, cunning creatures. Go... and be careful. Don't let it fool you.'" << std::endl;
}

void NPC::give_quest_necromancer() {
    std::cout << getName() << ": 'This is a dark omen. A **Necromancer** is defiling the ancient crypts to the west (102,19). This evil must be stopped before he raises an army of the dead and threatens the entire region. This is a top priority.'" << std::endl;
}

void NPC::give_quest_golem() {
    std::cout << getName() << ": 'The earth itself groans. A **Stone Golem**, a relic of a forgotten age, has awakened in the southern quarry (56,33). It is a force of pure destruction. This is not a hunt, warrior... this is a battle for survival. It must be stopped.'" << std::endl;
}
void NPC::complete_quest_war_chief() {
    std::cout << getName() << ": 'So, the War Chief is defeated. Good. You have done the village a service. Well done, warrior.'" << std::endl;
}

void NPC::complete_quest_orc_raider() {
    std::cout << getName() << ": 'The trade road is clear? Hmph. You are efficient. Take this for your trouble.'" << std::endl;
}

void NPC::complete_quest_infernal_imp() {
    std::cout << getName() << ": 'You've dealt with the Imp? Good riddance. I trust you weren't outsmarted by that little fiend.'" << std::endl;
}

void NPC::complete_quest_necromancer() {
    std::cout << getName() << ": 'The Necromancer... is gone? You have done the impossible. You have lifted a shadow that could have consumed us all. The entire region owes you a great debt.'" << std::endl;
}

void NPC::complete_quest_golem() {
    std::cout << getName() << ": 'You... stopped the Golem? To stand against such power and prevail... Your strength is truly legendary. I... I am in your debt. We are all in your debt.'" << std::endl;
}