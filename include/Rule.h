#ifndef RULE_H
#define RULE_H
#include <codecvt>
#include <locale>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Button.h"
#include "GlobalVar.h"
#include "Player.h"

class Rule {
private:
    Button ruleButton;         // 规则系统按钮
    Button quitButton;
    RectangleShape buttonMask; //蒙一层磨砂效果
    sf::RectangleShape ruleWindow;  // 规则界面的背景
    bool isRuleOpen;               // 规则系统是否打开
    Clock clickClock;
    const float clickCooldown = 1.0f;
    Font font;
public:
    Rule();
    void update(sf::RenderWindow& window,MovableObject&girl);
    void render(sf::RenderWindow& window);
    void handleClick(const sf::Vector2i& mousePos,MovableObject& girl);
    void openRule();
    void closeRule();
    bool isRuleActive() const { return isRuleOpen; }
    void setPos(float x,float y);
    void loadFont(const std::string& fontPath) {
        if (!font.loadFromFile(fontPath)) {
            std::cerr << "Failed to load font!" << std::endl;
        }
    }
};

#endif
