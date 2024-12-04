#include "../include/LevelState.h"
#include "../include/EndState.h"
#include "../include/Game.h"
#include "../include/TexturePool.h"



LevelState::LevelState(Game*game): game(game){
    if (game == nullptr) {
        throw std::runtime_error("Game pointer is null in LevelState constructor!");
    }
    loading = load_map("../configs/MapTwo.json");
    jsonArrayWidth = loading["layers"][0]["width"];
    jsonArrayHeight = loading["layers"][0]["height"];
    mapData = load_map_data(loading, jsonArrayWidth, jsonArrayHeight);
    Texture&texture= TexturePool::getTexture("../resources/images/movetest.png");
    
    background1=Map("../resources/images/MapTwo.png");
    background2=Map("../resources/images/MapThree.png");
    background2=Map("../resources/images/MapFour.png");
    isBg1=true;
    isBg2=false;
    isBg3=false;
    float sx=background1.returnScaleX();
    float sy=background1.returnScaleY();
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
            return;
        }
        if(girl.toExit(mapData,16.0*background1.returnScaleX(),16.0*background1.returnScaleY())){
            game->changeState(make_unique<EndState>(game));
            return;
        }
        if(girl.checkwater(mapData,16.0*background1.returnScaleX(),16.0*background1.returnScaleY())){
            cout<<111<<endl;
            game->changeState(make_unique<EndState>(game));
            return;
        }

    }
}

void LevelState::update(){
    changeMap=false;
    float deltaTime=clock.restart().asSeconds();//获取帧时间差
    girl.update(deltaTime,mapData,16.0*background1.returnScaleX(),16.0*background1.returnScaleY());
    if(girl.changeMap(mapData,16.0*background1.returnScaleX(),16.0*background1.returnScaleY())==7){
        isBg1=false;
        isBg2=true;
        changeMap=true;
    }else if(girl.changeMap(mapData,16.0*background1.returnScaleX(),16.0*background1.returnScaleY())==8){
        isBg1=true;
        isBg2=false;
        changeMap=true;
    }else if(girl.changeMap(mapData,16.0*background1.returnScaleX(),16.0*background1.returnScaleY())==9){
        isBg2=false;
        isBg3=true;
        changeMap=true;
    }else if(girl.changeMap(mapData,16.0*background1.returnScaleX(),16.0*background1.returnScaleY())==10){
        isBg2=true;
        isBg3=false;
        changeMap=true;
    }
}

void LevelState::render(RenderWindow& window){
    window.clear();
    if(isBg1)background1.draw(window);
    else if(isBg2)background2.draw(window);
    else if(isBg3)background3.draw(window);
    girl.render(window);
    window.display();
}
