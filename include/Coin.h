#ifndef COIN_H
#define COIN_H

#include "../SFML/Graphics.hpp"
#include "../SFML/System/Clock.hpp"
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
            isPicked = true;  // 拾取竖琴
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
            isPicked = true;  // 拾取小号
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
            isPicked = true;  // 拾取泡芙
            audio.playSoundEffect(SoundChannel::System,"fetch",SoundPriority::HIGH);
            girl.earnPuff();
            girl.addItemToBag(10);

        }
    }

};
class Note:public Coin{
    private:
    int id;
    public:
    Note(float x, float y, const sf::Texture& texture,int id):Coin(x,y,texture),id(id){

    }
    void checkCollision(MovableObject& girl)override{
        if (!isPicked && sprite.getGlobalBounds().intersects(girl.getBounds())) {
            isPicked = true;  // 拾取音符
            audio.playSoundEffect(SoundChannel::System,"fetch",SoundPriority::HIGH);
            if(id==2){
                noteOwnStatus[14]=true;
                acCheck[13]=true;
                acCheck[4]=true;
            }else if(id==3){
                noteOwnStatus[15]=true;
                acCheck[14]=true;
            }else if(id==4){
                noteOwnStatus[16]=true;
                acCheck[15]=true;
            }else if(id==5){
                noteOwnStatus[17]=true;
                acCheck[16]=true;
                acCheck[7]=true;
            }else if(id==6){
                noteOwnStatus[18]=true;
                acCheck[17]=true;
            }else if(id==7){
                noteOwnStatus[19]=true;
                acCheck[8]=true;
                acCheck[18]=true;
            }
            girl.addItemToBag(20);

        }
    }

};
// 猫咪
class Cat:public Coin{

    public:
    Cat(float x, float y, const sf::Texture& texture):Coin(x,y,texture){

    }
    void checkCollision(MovableObject& girl)override{
        if (!isPicked && sprite.getGlobalBounds().intersects(girl.getBounds())) {
            isPicked = true;
            acCheck[10]=true;
            bool temp=true;
            for(int i=0;i<=19;i++){
                if(items[i].quantity<1){
                    temp=false;
                }
                break;
            }
            acCheck[11]=temp;
            audio.playSoundEffect(SoundChannel::System,"idleSing",SoundPriority::HIGH);
            getCat=true;
        }
    }

};
#endif