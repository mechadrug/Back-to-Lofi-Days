#include "../include/MenuState.h"
#include "../include/Game.h"
#include "../include/LevelState.h"
#include <iostream>
using namespace std;
MenuState::MenuState(Game* game) : game(game) {

    // 获取窗口的大小
    sf::Vector2u windowSize = game->getWindow().getSize();

    // 按钮的宽度和高度
    unsigned int buttonWidth = 240;
    unsigned int buttonHeight = 80;

    // 水平居中：窗口宽度的一半减去按钮宽度的一半
    unsigned int startButtonX = (windowSize.x - buttonWidth) / 2;
    unsigned int startButtonY = windowSize.y - buttonHeight - 140; // 垂直方向上放在底部上方一定距离（160像素）

    unsigned int exitButtonX = (windowSize.x - buttonWidth) / 2;
    unsigned int exitButtonY = startButtonY + buttonHeight + 10; // Exit 按钮位于 Start 按钮下方，设置一定的间隔

    startButtonTexture = std::make_shared<sf::Texture>();
    if (!startButtonTexture->loadFromFile("../resources/images/start.png")) {
        throw runtime_error("Failed to load start button texture!");
    }

    exitButtonTexture = std::make_shared<sf::Texture>();
    if (!exitButtonTexture->loadFromFile("../resources/images/start.png")) {
        throw runtime_error("Failed to load exit button texture!");
    }

    // 初始化按钮，使用自定义纹理
    startButton = Button(startButtonX, startButtonY, buttonWidth, buttonHeight, startButtonTexture);
    exitButton = Button(exitButtonX, exitButtonY, buttonWidth, buttonHeight, exitButtonTexture);
    background = std::make_shared<Map>("../resources/images/realbg-menu.png");
}

void MenuState::handleInput(RenderWindow& window){
    Event event;
    while(window.pollEvent(event)){
        if(event.type==Event::Closed){
            window.close();
        }
        if (event.type == Event::Resized) {
            cout<<"re!"<<endl;
            //game->initView();
        }
        if(event.type == Event::MouseButtonPressed){
            if(startButton.isPressed(Mouse::getPosition(window))){
                cout<<"???"<<endl;
                game->changeState(make_unique<LevelState>(game));

            }
            if(exitButton.isPressed(Mouse::getPosition(window))) {
                cout<<"ExitButton clicked!"<<endl;
                window.close();
            }
        }
        
    }
}
void MenuState::update(){
}//更新菜单状态

void MenuState::render(RenderWindow& window){
    window.clear(Color::Black);
    background->draw(window);
    startButton.draw(window);
    exitButton.draw(window);
    window.display();
}