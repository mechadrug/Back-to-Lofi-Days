#ifndef SHOP_H
#define SHOP_H

#include <SFML/Graphics.hpp>
#include "Button.h"  // 引入 Button 类
#include "GlobalVar.h"
class Shop {
private:
    Button shopButton;         // 商店按钮
    sf::RectangleShape shopWindow;  // 商店界面的背景
    bool isShopOpen;               // 商店是否打开
    Clock clickClock;
    const float clickCooldown = 1.0f;
public:
    Shop();
    void update(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    void handleClick(const sf::Vector2i& mousePos);
    void openShop();
    void closeShop();
    bool isShopActive() const { return isShopOpen; }
};

#endif
