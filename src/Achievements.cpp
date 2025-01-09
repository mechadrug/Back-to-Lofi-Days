#include "../include/Achievements.h"

Achievements::Achievements() : isAcOpen(false), acButton(-50, 450, 50, 50, "../resources/images/hint.png") {
    // 设置商店界面的背景（简单的矩形，实际上你可以加载更复杂的界面）
    acWindow.setSize(sf::Vector2f(400, 300));
    acWindow.setFillColor(sf::Color(0, 0, 0, 200));  // 半透明黑色背景
    acWindow.setPosition(200, 100);  // 商店界面的初始位置
    buttonMask.setSize(Vector2f(50,50));
    buttonMask.setFillColor(Color(0, 0, 0, 200));
    buttonMask.setPosition(0, 450);
    acButton.setColor();
}

void Achievements::update(sf::RenderWindow& window) {
    // 检查鼠标点击并处理按钮交互
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        handleClick(sf::Mouse::getPosition(window));
    }

    // 如果商店打开，可以在这里添加更多的商店交互逻辑
}

void Achievements::render(sf::RenderWindow& window) {
    acButton.draw(window);  // 渲染商店按钮

    // 如果商店界面打开，绘制商店界面
    if (isAcOpen) {
        window.draw(acWindow);
    }
}

void Achievements::handleClick(const sf::Vector2i& mousePos) {
    if (clickClock.getElapsedTime().asSeconds() < clickCooldown) {
        return;  // 如果没过 1 秒，则不执行点击操作
    }
    
    // 如果鼠标点击的位置在商店按钮范围内
    if (acButton.isPressed(mousePos)) {
        if (isAcOpen) {
            closeShop();  // 关闭商店
            //gamePaused = false;  // 恢复游戏
            usingSystem=0;
        } else {
            openShop();  // 打开商店
            //gamePaused = true;  // 暂停游戏
            usingSystem=2;
        }
        clickClock.restart();
    }
}

void Achievements::openShop() {
    isAcOpen = true;
}

void Achievements::closeShop() {
    isAcOpen = false;
}

void Achievements::setPos(float x, float y){
    buttonMask.setPosition(x,y);
    acButton.setPos(x,y);
}
