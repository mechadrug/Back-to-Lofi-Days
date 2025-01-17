#ifndef COIN_H
#define COIN_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "Player.h"
#include "Map.h"

class Coin {
protected:
    sf::Sprite sprite;
    sf::Vector2f position;
    bool isPicked;  // 用于标记金币是否已被拾取
public:
    Coin(float x, float y, const sf::Texture& texture) 
        : position(x, y), isPicked(false) {
        sprite.setTexture(texture);
        sprite.setScale(globalScaleX,globalScaleY);
        sprite.setPosition(position);
    }

    // 判断角色是否在金币位置并拾取金币
    virtual void checkCollision(MovableObject& girl) {
        // 假设角色的尺寸为16x16像素
        if (!isPicked && sprite.getGlobalBounds().intersects(girl.getBounds())) {
            isPicked = true;  // 拾取金币
            audio.playSoundEffect(SoundChannel::System,"fetch",SoundPriority::HIGH);
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

class Harp:public Coin{

    public:
    Harp(float x, float y, const sf::Texture& texture):Coin(x,y,texture){
    }
    void checkCollision(MovableObject& girl)override{
        if (!isPicked && sprite.getGlobalBounds().intersects(girl.getBounds())) {
            isPicked = true;  // 拾取金币
            audio.playSoundEffect(SoundChannel::System,"fetch",SoundPriority::HIGH);
            girl.earnHarp();
            girl.addItemToBag(12);
        }
    }

};
class Trumpette:public Coin{
    public:
    Trumpette(float x, float y, const sf::Texture& texture):Coin(x,y,texture){
    }
    void checkCollision(MovableObject& girl)override{
        if (!isPicked && sprite.getGlobalBounds().intersects(girl.getBounds())) {
            isPicked = true;  // 拾取金币
            audio.playSoundEffect(SoundChannel::System,"fetch",SoundPriority::HIGH);
            girl.earnTrumpette();
            girl.addItemToBag(11);

        }
    }

};
class Puff:public Coin{

    public:
    Puff(float x, float y, const sf::Texture& texture):Coin(x,y,texture){

    }
    void checkCollision(MovableObject& girl)override{
        if (!isPicked && sprite.getGlobalBounds().intersects(girl.getBounds())) {
            isPicked = true;  // 拾取金币
            audio.playSoundEffect(SoundChannel::System,"fetch",SoundPriority::HIGH);
            girl.earnPuff();
            girl.addItemToBag(10);

        }
    }

};
#endif