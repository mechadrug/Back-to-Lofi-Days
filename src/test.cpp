#include <SFML/Graphics.hpp>
#include <iostream>

class Button1 {
public:
    Button1(float x, float y, float width, float height, const sf::Color& bgColor) {
        shape.setPosition(x, y);
        shape.setSize({width, height});
        shape.setFillColor(bgColor); // 设置背景颜色
        shape.setOutlineThickness(1.0f);
        shape.setOutlineColor(sf::Color::White); // 边框颜色
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape); // 绘制按钮
    }

private:
    sf::RectangleShape shape;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Button Test");

    Button1 button(300, 200, 200, 50, sf::Color::Blue); // 蓝色按钮

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        button.draw(window); // 绘制按钮
        window.display();
    }

    return 0;
}
