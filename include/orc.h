#ifndef ORC_H
#define ORC_H
#include "enemy.h" 
#include <chrono>
#include <string>
#include <vector>

class Orc : public Enemy 
{
public:
    Orc();
    std::vector<Item> getLoot(const Player& player) const override;
    virtual void triggerDialogue() const override;
};
#endif