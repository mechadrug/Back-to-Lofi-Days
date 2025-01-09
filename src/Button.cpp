#include "../include/Button.h"
#include "../include/TexturePool.h"
#include <iostream>

Button::Button(float x, float y, float width, float height, const std::string& textureFile)
{
    Texture&texture = TexturePool::getTexture(textureFile);
    sprite.setTexture(texture);
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;

    sprite.setPosition(x, y);

    sprite.setScale(width / texture.getSize().x, height / texture.getSize().y);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(sprite);  // 绘制按钮的图片
}

bool Button::isPressed(sf::Vector2i mousePosition) {
    // 检查鼠标是否点击了按钮
    return sprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y);
}

sf::FloatRect Button::getBounds() const {
    return sprite.getGlobalBounds();
}

void Button::setPos(float x, float y){
    sprite.setPosition(x,y);
}

