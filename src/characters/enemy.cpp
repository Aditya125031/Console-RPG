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