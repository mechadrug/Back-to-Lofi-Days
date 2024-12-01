#include "../include/MenuState.h"
#include "../include/Game.h"
#include "../include/LevelState.h"
#include "../include/TexturePool.h"
#include <iostream>
using namespace std;
 MenuState::MenuState(Game* game) : game(game) {

    //获取窗口的大小
    sf::Vector2u windowSize = game->getWindow().getSize();

    //开始按钮的宽度和高度:
     unsigned int buttonWidth = 240;
     unsigned int buttonHeight = 80;
    //结束按钮的宽度:
    unsigned int exitButttonWidth = 120;
    unsigned int bgwidth=240;
    unsigned int bgheight=80;

    unsigned int startButtonX = (windowSize.x - buttonWidth) / 2;
    unsigned int startButtonY = windowSize.y - buttonHeight - 140;

    unsigned int exitButtonX = (windowSize.x - exitButttonWidth) / 2;
    unsigned int exitButtonY = startButtonY + buttonHeight + 10;
    startButton=Button(startButtonX,startButtonY,buttonWidth,buttonHeight,"../resources/images/start.png");
    exitButton=Button(exitButtonX,exitButtonY,exitButttonWidth,buttonHeight,"../resources/images/exit.png");
    background=Map("../resources/images/menuBg.png");
 }

void MenuState::handleInput(RenderWindow& window){
    Event event;
    while(window.pollEvent(event)){
        if(event.type==Event::Closed){
            window.close();
        }
        if (event.type == Event::Resized) {
            cout<<"re!"<<endl;
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
    background.draw(window);
    startButton.draw(window);
    exitButton.draw(window);   
    window.display();
}
