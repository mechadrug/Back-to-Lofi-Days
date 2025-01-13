#include "../include/Rule.h"

Rule::Rule() : isRuleOpen(false), ruleButton(-50, 550, 50, 50, "../resources/images/rule.png") {
    // 设置商店界面的背景（简单的矩形，实际上你可以加载更复杂的界面）
    fx=window_sz.x/2-300.f;
    fy=window_sz.y/2-225.f;
    ruleWindow.setSize(sf::Vector2f(600, 450));
    ruleWindow.setFillColor(sf::Color(0, 0, 0, 200));  // 半透明黑色背景
    ruleWindow.setPosition(fx, fy);  // 商店界面的初始位置
    buttonMask.setSize(Vector2f(50,50));
    buttonMask.setFillColor(Color(0, 0, 0, 200));
    buttonMask.setPosition(-50, 550); // init pos out of window
    ruleButton.setColor();
    loadFont("../resources/fonts/Silver.ttf");

}

void Rule::update(sf::RenderWindow& window,MovableObject&girl) {
    // 检查鼠标点击并处理按钮交互
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        handleClick(sf::Mouse::getPosition(window),girl);
    }
    
}

void Rule::render(sf::RenderWindow& window) {
    window.draw(buttonMask);
    ruleButton.draw(window);  // 渲染规则按钮
    

    // 如果规则界面打开，绘制商店界面
    if (isRuleOpen) {
        window.draw(ruleWindow);
        Text head;
        head.setFont(font);
        head.setString(gameRules[0]);
        head.setCharacterSize(46);
        head.setFillColor(Color::White);
        head.setPosition(fx+260.f,fy-20.f);
        Text line;
        line.setFont(font);
        window.draw(head);
        for(int i=1;i<=13;i++){
            line.setString(gameRules[i]);
            line.setCharacterSize(24);
            line.setFillColor(Color::White);
            line.setPosition(fx+40.f,fy+32.f*i);
            window.draw(line);
        }
    }
}

void Rule::handleClick(const sf::Vector2i& mousePos,MovableObject& girl) {
    if (clickClock.getElapsedTime().asSeconds() < clickCooldown) {
        return;  // 如果没过 1 秒，则不执行点击操作
    }

    
    // 如果鼠标点击的位置在Rule按钮范围内
    if (ruleButton.isPressed(mousePos)) {
        if (isRuleOpen) {
            closeRule();  // 关闭rule
            usingSystem=0;
        } else {
            openRule();  // 打开rule
            usingSystem=4;
        }
        clickClock.restart();
    }
    
}

void Rule::openRule() {
    isRuleOpen = true;
}

void Rule::closeRule() {
    isRuleOpen = false;
}

void Rule::setPos(float x,float y){
    buttonMask.setPosition(x,y);
    ruleButton.setPos(x,y);
}
