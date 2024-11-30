#include "../include/LevelState.h"
#include "../include/EndState.h"
#include "../include/Game.h"
#include "../include/TexturePool.h"



LevelState::LevelState(Game*game): game(game){
    if (game == nullptr) {
        throw std::runtime_error("Game pointer is null in LevelState constructor!");
    }
    loading = load_map("../configs/testmap.json");
    jsonArrayWidth = loading["layers"][0]["width"];
    jsonArrayHeight = loading["layers"][0]["height"];
    mapData = load_map_data(loading, jsonArrayWidth, jsonArrayHeight);
    Texture&texture= TexturePool::getTexture("../resources/images/movetest.png");
    
    background=Map("../resources/images/MapOne.png");
    float sx=background.returnScaleX();
    float sy=background.returnScaleY();
    float gsx=32.f*sx;
    float gsy=16.f*sy;
    girl = MovableObject(gsx,gsy,texture,sx,sy);
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

void LevelState::update(){
    float deltaTime=clock.restart().asSeconds();//获取帧时间差
    girl.update(deltaTime,mapData,16*background.returnScaleX(),16*background.returnScaleY());
}

void LevelState::render(RenderWindow& window){
    window.clear();
    background.draw(window);
    girl.render(window);
    window.display();
}
