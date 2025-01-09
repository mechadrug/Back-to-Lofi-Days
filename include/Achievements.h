#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include <SFML/Graphics.hpp>
#include "Button.h"  // 引入 Button 类
#include "GlobalVar.h"
class Achievements {
private:
    Button acButton;         // 成就按钮
    RectangleShape buttonMask; //蒙一层磨砂效果
    sf::RectangleShape acWindow;  // 成就界面的背景
    bool isAcOpen;               // 成就是否打开
    Clock clickClock;
    const float clickCooldown = 1.0f;
public:
    Achievements();
    void update(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    void handleClick(const sf::Vector2i& mousePos);
    void openShop();
    void closeShop();
    bool isShopActive() const { return isAcOpen; }
    void setPos(float x,float y);

};

#endif