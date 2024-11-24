#include "../include/LevelState.h"
#include "../include/EndState.h"
#include "../include/Game.h"

LevelState::LevelState(Game*game): game(game){}
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
    window.display();
}