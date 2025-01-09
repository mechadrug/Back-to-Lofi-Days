#ifndef COIN_H
#define COIN_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "Player.h"
#include "Map.h"

class Coin {
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    bool isPicked;  // 用于标记金币是否已被拾取
public:
    Coin(float x, float y, const sf::Texture& texture) 
        : position(x, y), isPicked(false) {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    // 判断角色是否在金币位置并拾取金币
    void checkCollision(MovableObject& girl) {
        // 假设角色的尺寸为16x16像素
        if (!isPicked && sprite.getGlobalBounds().intersects(girl.getBounds())) {
            isPicked = true;  // 拾取金币
            girl.earnMoney();
        }
    }

    void render(sf::RenderWindow& window) {
        if (!isPicked) {  // 只有未拾取的金币才渲染
            window.draw(sprite);
        }
    }
    bool hasBeenPicked() const {
        return isPicked;
    }
};

#endif