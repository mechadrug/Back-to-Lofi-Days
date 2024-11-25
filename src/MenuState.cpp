#include "../include/MenuState.h"
#include "../include/Game.h"
#include "../include/LevelState.h"
#include <iostream>
using namespace std;
MenuState::MenuState(Game* game) : game(game) {
    if (!font.loadFromFile("../resources/fonts/hk4e_zh-cn.ttf")) {
        throw runtime_error("Failed to load font!");
    }

    //字体加载成功后再初始化按钮
    startButton = Button(300, 200, 200, 50, font, "Start", sf::Color::Blue, sf::Color::White);
    exitButton = Button(300, 300, 200, 50, font, "Exit", sf::Color::Blue, sf::Color::White);
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
            if(startButton.isClicked(event.mouseButton)){
                game->changeState(make_unique<LevelState>(game));
            }
            if(exitButton.isClicked(event.mouseButton)) {
                cout<<"ExitButton clicked!"<<endl;
                window.close();
            }
        }
        
    }
}
void MenuState::update(){
    Vector2i mousePosition = Mouse::getPosition(game->getWindow());
    Vector2f worldPosition = game->getWindow().mapPixelToCoords(mousePosition);

    startButton.updateHover(worldPosition);
    exitButton.updateHover(worldPosition);//悬停效果
}//更新菜单状态

void MenuState::render(RenderWindow& window){
    window.clear(Color::Black);
    startButton.draw(window);
    exitButton.draw(window);
    window.display();
}