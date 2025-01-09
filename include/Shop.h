#ifndef SHOP_H
#define SHOP_H
#include <codecvt>
#include <locale>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Button.h"  // 引入 Button 类
#include "GlobalVar.h"
#include "Player.h"
#include "Backpack.h"
class Shop {
private:
    Button shopButton;         // 商店按钮
    RectangleShape buttonMask; //蒙一层磨砂效果
    sf::RectangleShape shopWindow;  // 商店界面的背景
    bool isShopOpen;               // 商店是否打开
    Clock clickClock;
    const float clickCooldown = 1.0f;
    Font font;
    vector<ItemButton> itemButtons; // 物品按钮列表
    string currentItemInfo; // 物品信息
public:
    Shop();
    void loadItems();
    void update(sf::RenderWindow& window,MovableObject&girl);
    void render(sf::RenderWindow& window);
    void handleClick(const sf::Vector2i& mousePos,MovableObject& girl);
    void showItemInfo(const sf::Vector2i& mousePos, sf::RenderWindow& window);
    void openShop();
    void closeShop();
    bool isShopActive() const { return isShopOpen; }
    void setPos(float x,float y);
    void loadFont(const std::string& fontPath) {
        if (!font.loadFromFile(fontPath)) {
            std::cerr << "Failed to load font!" << std::endl;
        }
    }
};

#endif
