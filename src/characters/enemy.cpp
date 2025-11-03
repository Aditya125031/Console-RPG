#include "../include/enemy.h"
using namespace std;

Enemy::Enemy(std::string name, int health, int attackPower)
    : Character(name, health, attackPower)
{
    this->normal_attack_ready = std::chrono::steady_clock::now();
}

std::string Enemy::get_name() const 
{
    return this->name; 
}
std::chrono::steady_clock::time_point Enemy::getNormalAttackReady() const {
    return this->normal_attack_ready; // Return the variable
}
void Enemy::setNormalAttackCooldown(float seconds) {
    this->normal_attack_ready = std::chrono::steady_clock::now() +  // Assign to the variable
        std::chrono::microseconds(static_cast<int>(seconds * 2000000)); // 1.0 second
}
void Enemy::setNormalAttackInterval(float seconds) {
    attackInterval = seconds;
    return;
}