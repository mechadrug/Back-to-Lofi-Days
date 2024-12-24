#include "../include/Slime.h"

Slime::Slime(float x, float y, const sf::Texture& texture, int health, 
             int attackDamage, float attackCooldown, int detectionRange)
    : movable(x, y, texture, 1.8f, 1.8f), health(health), attackDamage(attackDamage),
      attackCooldown(attackCooldown), detectionRange(detectionRange), isDead(false) {}
//注意这里的1.8f是暂时值之后再改动
void Slime::update(MovableObject& target) {
    if(gamePaused){
        return;
    }
    // 检测是否有目标角色在附近
    float slimeX = movable.getPosition().x;
    float slimeY = movable.getPosition().y;

    float targetX = target.getPosition().x;
    float targetY = target.getPosition().y;

    // 检测目标是否在攻击范围内（上下左右1格）
    if (abs(slimeX - targetX) <= 1.8f*detectionRange * 16.0f && abs(slimeY - targetY) <= 1.8f*detectionRange * 16.0f) {
        // 触发攻击
        if (attackClock.getElapsedTime().asSeconds() >= attackCooldown&&this->isAlive()) {
            attack(target);
            cout<<"Bob attack girl!"<<endl;
            attackClock.restart();
        }
    }
}

void Slime::attack(MovableObject& target) {
    // 攻击逻辑
    if(target.InvisibleForSlime()){
        cout<<"I cant see you"<<endl;
        return;
    }
    target.takeDamage(attackDamage);
}

void Slime::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        isDead = true;
    }
}

void Slime::render(sf::RenderWindow& window) {
    if (!isDead) {
        movable.render(window);
    }
}

bool Slime::isAlive() const {
    return !isDead;
}

sf::Vector2f Slime::getPosition() const {
    return movable.getPosition();  // 返回MovableObject的位置
}
