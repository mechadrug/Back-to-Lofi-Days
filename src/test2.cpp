#include <SFML/Graphics.hpp>
#include <iostream>
#include "../include/Button.h" // 假设你已实现 Button 类

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fixed Logical Resolution");
    sf::View view;

    sf::Font font;
    if(!font.loadFromFile("../resources/fonts/hk4e_zh-cn.ttf")){
        throw runtime_error("Failed to load font!");
    }

    Button startButton(300, 200, 200, 50, font, "Start", sf::Color::Black, sf::Color::White);
    Button exitButton(300, 300, 200, 50, font, "Exit", sf::Color::Black, sf::Color::White);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        startButton.draw(window);     // 绘制按钮
        exitButton.draw(window);      // 绘制按钮
        window.display();
    }

    return 0;
}
