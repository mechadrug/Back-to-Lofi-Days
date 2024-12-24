#include "../include/Shop.h"

Shop::Shop() : isShopOpen(false), shopButton(10, 600, 50, 50, "../resources/images/shop.png") {
    // 设置商店界面的背景（简单的矩形，实际上你可以加载更复杂的界面）
    shopWindow.setSize(sf::Vector2f(400, 300));
    shopWindow.setFillColor(sf::Color(0, 0, 0, 200));  // 半透明黑色背景
    shopWindow.setPosition(200, 100);  // 商店界面的初始位置
}

void Shop::update(sf::RenderWindow& window) {
    // 检查鼠标点击并处理按钮交互
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        handleClick(sf::Mouse::getPosition(window));
    }

    // 如果商店打开，可以在这里添加更多的商店交互逻辑
}

void Shop::render(sf::RenderWindow& window) {
    shopButton.draw(window);  // 渲染商店按钮

    // 如果商店界面打开，绘制商店界面
    if (isShopOpen) {
        window.draw(shopWindow);
    }
}

void Shop::handleClick(const sf::Vector2i& mousePos) {
    if (clickClock.getElapsedTime().asSeconds() < clickCooldown) {
        return;  // 如果没过 1 秒，则不执行点击操作
    }
    
    // 如果鼠标点击的位置在商店按钮范围内
    if (shopButton.isPressed(mousePos)) {
        if (isShopOpen) {
            closeShop();  // 关闭商店
            gamePaused = false;  // 恢复游戏
        } else {
            openShop();  // 打开商店
            gamePaused = true;  // 暂停游戏
        }
        clickClock.restart();
    }
}

void Shop::openShop() {
    isShopOpen = true;
}

void Shop::closeShop() {
    isShopOpen = false;
}
