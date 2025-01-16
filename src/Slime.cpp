#include "../include/Slime.h"

Slime::Slime(float x, float y, const sf::Texture& texture, int health, 
             int attackDamage, float attackCooldown, int detectionRange)
    : movable(x, y, texture, 1.8f, 1.8f,1), health(health), attackDamage(attackDamage),
      attackCooldown(attackCooldown), detectionRange(detectionRange), isDead(false){}
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
            audio.playSoundEffect(SoundChannel::Monster,"bobSlimeATK",SoundPriority::MEDIUM);
            attack(target);
            cout<<"Bob attack girl!"<<endl;
            attackClock.restart();
        }
    }
}

void Slime::attack(MovableObject& target) {
    // 攻击逻辑
    if(target.InvisibleForSlime()){
        cout<<"I can't see you"<<endl;
        return;
    }
    target.takeDamage(attackDamage);
}

void Slime::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        isDead = true;
        audio.playSoundEffect(SoundChannel::Monster,"bobSlimeDie",SoundPriority::HIGH);
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


RandomWalkingSlime::RandomWalkingSlime(float x, float y, const sf::Texture& texture, int health, 
                                       int attackDamage, float attackCooldown, int detectionRange, 
                                       float moveCooldown, float moveSpeed)
    : Slime(x, y, texture, health, attackDamage, attackCooldown, detectionRange),
      moveCooldown(moveCooldown), moveSpeed(moveSpeed),isVerticalMovement(true)
{   initialPosition = movable.getPosition();
    movingPositive=false;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);
    isVerticalMovement = dist(gen) == 0;
    movementClock.restart();
}

void RandomWalkingSlime::update(MovableObject& target) {
    Slime::update(target);  // 调用父类的update方法

    if (gamePaused) {
        return;
    }

     // 检查是否需要切换移动方向
    if (movementClock.getElapsedTime().asSeconds() >= moveCooldown) {
        movementClock.restart();
        movingPositive = !movingPositive;  // 切换移动方向
        std::cout << "Slime switched direction to " << (movingPositive ? "positive" : "negative") << std::endl;
    }

    // 调用平滑移动函数
    moveTowardsTarget(DELTATIME);
}

void RandomWalkingSlime::moveTowardsTarget(float deltaTime) {
    Vector2f movement(0.f, 0.f);
    if (isVerticalMovement) {
        movement.y = movingPositive ? -moveSpeed * deltaTime : moveSpeed * deltaTime;
    } else {
        movement.x = movingPositive ? -moveSpeed * deltaTime : moveSpeed * deltaTime;
    }
    Vector2f newpos=movement;
    movable.setPosition(movement);
    
}

Missile::Missile(float x, float y, const sf::Texture& texture, MissileDirection direction, float speed, float maxDistance)
    : direction(direction), speed(speed), maxDistance(maxDistance), distanceTraveled(0.f) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    startPosition = sf::Vector2f(x, y);
}

void Missile::update(float deltaTime) {
    sf::Vector2f movement(0.f, 0.f);
    if (direction == MissileDirection::Left) {
        movement.x -= speed * deltaTime;
    } else {
        movement.x += speed * deltaTime;
    }
    sprite.move(movement);
    distanceTraveled += std::abs(movement.x);
}

void Missile::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Missile::isExpired() const {
    return distanceTraveled >= maxDistance;
}

bool Missile::checkCollision(MovableObject& player) {
    return sprite.getGlobalBounds().intersects(player.getGlobalBounds());
}

sf::Vector2f Missile::getPosition() const {
    return sprite.getPosition();
}



MissileSlime::MissileSlime(float x, float y, const sf::Texture& texture, const sf::Texture& missileTextureLeft,
                            const sf::Texture& missileTextureRight, 
                           int health, int attackDamage, float attackCooldown, 
                           int detectionRange, float missileCooldown)
    : Slime(x, y, texture, health, attackDamage, attackCooldown, detectionRange),
      extendedDetectionRange(detectionRange * 2), // 扩展检测范围（例如，原来的两倍）
      missileCooldown(missileCooldown),
      missileTextureLeft(missileTextureLeft),missileTextureRight(missileTextureRight) {}

void MissileSlime::update(MovableObject& target) {
    if (isDead) {
        return;
    }

    // 获取史莱姆和玩家的位置
    sf::Vector2f slimePos = getPosition();
    sf::Vector2f playerPos = target.getPosition();

    // 计算水平和垂直距离
    float dx = playerPos.x - slimePos.x;
    float dy = playerPos.y - slimePos.y;

    // 计算欧几里得距离
    float distance = std::sqrt(dx * dx + dy * dy);

    // 检测玩家是否在扩展后的检测范围内
    if (distance <= extendedDetectionRange) {
        // 检测导弹发射冷却
        if (missileClock.getElapsedTime().asSeconds() >= missileCooldown) {
            // 发射导弹
             fireMissiles();
            missileClock.restart();
        }
    }

    // 更新导弹
    for (auto it = missiles.begin(); it != missiles.end(); ) {
        (*it)->update(DELTATIME);

        // 检测是否与玩家碰撞
        if ((*it)->checkCollision(target)) {
            std::cout << "Player hit by missile!" << std::endl;
            target.takeDamage(attackDamage); // 导弹伤害
            it = missiles.erase(it); // 移除导弹
            continue;
        }

        // 检查导弹是否过期
        if ((*it)->isExpired()) {
            it = missiles.erase(it); // 移除导弹
            continue;
        }

        ++it;
    }

}

void MissileSlime::render(sf::RenderWindow& window) {
    if (!isDead) {
        movable.render(window);
    }

    // 渲染导弹
    for (auto& missile : missiles) {
        missile->render(window);
    }
}

void MissileSlime::fireMissiles() {
    std::cout << "MissileSlime at (" << getPosition().x << ", " << getPosition().y << ") fires missiles!" << std::endl;

    // 发射左边导弹
    missiles.emplace_back(std::make_unique<Missile>(
        getPosition().x, getPosition().y, missileTextureLeft, MissileDirection::Left));

    // 发射右边导弹
    missiles.emplace_back(std::make_unique<Missile>(
        getPosition().x, getPosition().y, missileTextureRight, MissileDirection::Right));
}