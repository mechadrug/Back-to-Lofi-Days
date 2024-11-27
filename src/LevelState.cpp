#include "../include/LevelState.h"
#include "../include/EndState.h"
#include "../include/Game.h"
#include<iostream>
LevelState::LevelState(Game*game): game(game){
    if (game == nullptr) {
        throw std::runtime_error("Game pointer is null in LevelState constructor!");
    }
    if (!font.loadFromFile("../resources/fonts/hk4e_zh-cn.ttf")) {
            throw runtime_error("Failed to load font!");
        }

        // 创建文本对象，显示进入 LevelState 的信息
        message.setFont(font);
        message.setString("Entered LevelState!");
        message.setCharacterSize(30); // 字体大小
        message.setFillColor(Color::White);
        message.setPosition(100, 100); // 文本位置
    std::cout << "Entering LevelState..." << std::endl;
}
void LevelState::handleInput(RenderWindow& window){
    Event event;
    while(window.pollEvent(event)){
        if(event.type==Event::Closed){
            window.close();
        }
        if(event.type==Event::KeyPressed){
            if(event.key.code==Keyboard::Enter){
                game->changeState(make_unique<EndState>(game));
            }
        }
    }
}

void LevelState::update(){}

void LevelState::render(RenderWindow& window){
    window.clear();
    window.draw(message);
    window.display();
}
