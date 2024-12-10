#ifndef SLIME_H
#define SLIME_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "Player.h"

class Slime {
private:
    MovableObject movable;           // 使用MovableObject来管理位置和渲染
    int health;                      // Slime的血量
    bool isDead;                     // 是否死亡
    sf::Clock attackClock;           // 攻击计时器
    float attackCooldown;            // 攻击冷却时间
    float attackDamage;              // 攻击伤害
    int detectionRange;              // 检测范围（每方向一格）

public:
    Slime(float x, float y, const sf::Texture& texture, int health = 100, 
          float attackDamage = 10.f, float attackCooldown = 1.f, int detectionRange = 1);

    void update(std::vector<std::vector<Tile>>& mapData, MovableObject& target);
    void attack(MovableObject& target);
    void takeDamage(int damage);
    void render(sf::RenderWindow& window);

    bool isAlive() const;
    sf::Vector2f getPosition() const;
};

#endif
