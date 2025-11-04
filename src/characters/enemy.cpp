#include "../include/enemy.h"
using namespace std;

Enemy::Enemy(std::string name, int health, int attackPower)
    : Character(name, health, attackPower)
{}
std::string Enemy::get_name() const 
{
    return this->name; 
}
double Enemy::getNormalAttackInterval() const{
    return this->normalAttackInterval;
}
void Enemy::setNormalAttackInterval(double sec) {
    this->normalAttackInterval = sec;
    return;
}
void Enemy::setNormalAttackCooldown(float seconds) {
    this->normal_attack_ready = std::chrono::steady_clock::now() +  // Assign to the variable
        std::chrono::microseconds(static_cast<int>(seconds * 2000000)); // 1.0 second
}
std::vector<Item> Enemy::getLoot(const Player& player) const
{
    (void)player;
    return this->dropLoot; 
}