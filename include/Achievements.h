#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Player.h"
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
    Font font;
    vector<ACButton>acButtons;// 处理点击
    vector<ACButton>showButtons;// 专门显示
    // wstring currentAcInfo;
public:
    Achievements();
    void loadAC();
    void update(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    void handleClick(const sf::Vector2i& mousePos);
    void showACInfo(const sf::Vector2i& mousePos);
    void openShop();
    void closeShop();
    bool isShopActive() const { return isAcOpen; }
    void setPos(float x,float y);
    void loadFont(const std::string& fontPath) {
        if (!font.loadFromFile(fontPath)) {
            std::cerr << "Failed to load font!" << std::endl;
        }
    }
};

#endif