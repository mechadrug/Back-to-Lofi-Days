#include "../include/Button.h"
#include <iostream>

Button::Button(float x, float y, float width, float height, const std::shared_ptr<sf::Texture>& texture)
    : x(x), y(y), width(width), height(height) {
    
    if (!texture) {
    throw std::runtime_error("Texture is null in Button constructor");
    }
    // 设置按钮精灵
    sprite.setTexture(*texture);
    sprite.setPosition(x, y);
    if (sprite.getLocalBounds().width != 0 && sprite.getLocalBounds().height != 0) {
        sprite.setScale(width / sprite.getLocalBounds().width, height / sprite.getLocalBounds().height);
    } else {
        std::cerr << "Invalid sprite dimensions!" << std::endl;
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(sprite);  // 绘制按钮的图片
}

bool Button::isPressed(sf::Vector2i mousePosition) {
    // 检查鼠标是否点击了按钮
    return sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}

sf::FloatRect Button::getBounds() const {
    return sprite.getGlobalBounds();
}
