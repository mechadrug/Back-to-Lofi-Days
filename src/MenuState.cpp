#include "../include/MenuState.h"
#include "../include/Game.h"
#include "../include/LevelState.h"

MenuState::MenuState(Game* game):game(game),startButton(300,200,200,50,font,"Start"){
    if(!font.loadFromFile("../resources/fonts/hk4e_zh-cn.ttf")){
        throw runtime_error("Failed to load font!");
    }
}

void MenuState::handleInput(RenderWindow& window){
    Event event;
    while(window.pollEvent(event)){
        if(event.type==Event::Closed){
            window.close();
        }
        if(event.type == Event::MouseButtonPressed){
            if(startButton.isClicked(event.mouseButton)){
                game->changeState(make_unique<LevelState>(game));
            }
        }
    }
}
void MenuState::update(){}

void MenuState::render(RenderWindow& window){
    window.clear();
    startButton.draw(window);
    window.display();
}