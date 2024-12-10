#include "../include/LevelState.h"
#include "../include/EndState.h"
#include "../include/Game.h"
#include "../include/TexturePool.h"



LevelState::LevelState(Game*game): game(game),manager(){
    if (game == nullptr) {
        throw std::runtime_error("Game pointer is null in LevelState constructor!");
    }
    manager.loadBackgrounds({"../resources/images/MapTwo.png", "../resources/images/MapThree.png", "../resources/images/MapFour.png"});
    
    manager.loadMapData("../configs/Map1.json");
    Texture&texture= TexturePool::getTexture("../resources/images/movetest.png");

    float sx=manager.getCurrentBackground().returnScaleX();
    float sy=manager.getCurrentBackground().returnScaleY();
    float gsx=32.f*sx;
    float gsy=16.f*sy;
    girl = MovableObject(gsx,gsy,texture,sx,sy);
    
    
    //计算窗口四个角的位置
    Vector2u windowSize = game->getWindow().getSize();
    topLeft=Vector2f(0.f, 0.f);  // 左上角
    topRight=Vector2f(static_cast<float>(windowSize.x), 0.f);  // 右上角
    bottomLeft=Vector2f(0.f, static_cast<float>(windowSize.y));  // 左下角
    bottomRight=Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));  // 右下角

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
        if(girl.toExit(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())){
            game->changeState(make_unique<EndState>(game));
            return;
        }
        if(girl.checkwater(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())){
            cout<<111<<endl;
            game->changeState(make_unique<EndState>(game));
            return;
        }

    }
}

void LevelState::update(){

    float deltaTime=clock.restart().asSeconds();//获取帧时间差
    girl.update(deltaTime,manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY());
    
    if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==7){
        float gsx=32.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomLeft.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(1,girl,gsx,gsy);
    }
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==8){
        float gsx=bottomRight.x-48.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomRight.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(0,girl,gsx,gsy);
    }
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==9){
        float gsx=topRight.x-32.f*manager.getCurrentBackground().returnScaleX();
        float gsy=16.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(2,girl,gsx,gsy);
    }
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==10){
        float gsx=bottomRight.x-64.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomRight.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(1,girl,gsx,gsy);
    }
}

void LevelState::render(RenderWindow& window){
    window.clear();
    manager.renderMap(window,manager.getCurrentMapData());
    girl.render(window);
    window.display();
}
