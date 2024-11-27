#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include<memory>
#include "../include/TexturePool.h"
class Button {
public:
    // 构造函数
    Button()=default;
    Button(float x, float y, float width, float height, const std::string& textureFile);
    // 渲染按钮
    void draw(sf::RenderWindow& window);
    // 检查鼠标点击是否在按钮区域内
    bool isPressed(sf::Vector2i mousePosition);
    // 获取按钮的位置和尺寸
    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;          // 用于显示按钮的精灵

    float width, height;
    float x, y;
};

#endif

