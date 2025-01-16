#ifndef SLIME_H
#define SLIME_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "Player.h"
#include "Map.h"
#include <random>

class Slime {
protected:
    MovableObject movable;           // 使用MovableObject来管理位置和渲染
    int health;                      // Slime的血量
    bool isDead;                     // 是否死亡
    sf::Clock attackClock;           // 攻击计时器
    float attackCooldown;            // 攻击冷却时间
    int attackDamage;              // 攻击伤害
    int detectionRange;              // 检测范围（每方向一格）

public:
    Slime(float x, float y, const sf::Texture& texture, int health = 8, 
          int attackDamage = 1, float attackCooldown = 1.f, int detectionRange = 2);

    virtual void update(MovableObject& target);
    void attack(MovableObject& target);
    void takeDamage(int damage);
    virtual void render(sf::RenderWindow& window);

    bool isAlive() const;
    sf::Vector2f getPosition() const;
};

class RandomWalkingSlime : public Slime {
private:
    sf::Clock movementClock;      // 控制随机移动的计时器
    float moveCooldown;           // 移动冷却时间
    float moveSpeed;              // 平滑移动的速度
    //sf::Vector2f targetPosition;  // 当前目标位置
    bool isVerticalMovement;      // 是否垂直移动（上下移动）
    bool movingPositive;          // 当前是否向正方向移动（向上或向右）
    //std::random_device rd;        // 用于生成随机数的设备
    //std::mt19937 gen;             // Mersenne Twister 随机数生成器
    //std::uniform_int_distribution<> dist; // 随机方向生成器
    sf::Vector2f initialPosition; // 初始位置

public:
    RandomWalkingSlime(float x, float y, const sf::Texture& texture, int health = 8, 
                        int attackDamage = 1, float attackCooldown = 1.f, int detectionRange = 1, 
                        float moveCooldown = 1.f, float moveSpeed = 150.f);

    void update(MovableObject& target) override;  // 重写update方法
    void moveTowardsTarget(float deltaTime);       // 平滑移动
};


enum class MissileDirection {
    Left,
    Right
};

class Missile {
public:
    Missile(float x, float y, const sf::Texture& texture, MissileDirection direction, float speed = 300.f, float maxDistance = 400.f);

    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool isExpired() const;
    bool checkCollision(MovableObject& player);

    sf::Vector2f getPosition() const;

private:
    sf::Sprite sprite;
    sf::Vector2f startPosition;
    MissileDirection direction;
    float speed;
    float maxDistance;
    float distanceTraveled;
};

class MissileSlime : public Slime {
private:
    int extendedDetectionRange; // 扩展后的检测范围
    std::vector<std::unique_ptr<Missile>> missiles; // 存储导弹的容器
    const float missileCooldown; // 导弹发射冷却时间
    sf::Clock missileClock; // 导弹发射计时器
    const sf::Texture& missileTextureLeft; // 导弹纹理引用
    const sf::Texture& missileTextureRight; // 导弹纹理引用

    void fireMissiles();
public:
    MissileSlime(float x, float y, const sf::Texture& texture, const sf::Texture& missileTextureLeft, const sf::Texture& missileTextureRight,
                int health = 8, int attackDamage = 1, float attackCooldown = 1.f, 
                int detectionRange = 200, float missileCooldown = 2.f);

    void update(MovableObject& target) override;
    void render(sf::RenderWindow& window) override;
};
#endif
