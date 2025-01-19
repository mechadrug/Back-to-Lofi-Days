#include "../include/LevelState.h"
#include "../include/EndState.h"
#include "../include/Game.h"
#include "../include/TexturePool.h"
#include "../include/AudioManager.h"
#include "../include/LoadAndStore.h"

LevelState::LevelState(Game*game): game(game),manager(),shop(){
    if (game == nullptr) {
        throw std::runtime_error("Game pointer is null in LevelState constructor!");
    }
    manager.loadBackgrounds({"../resources/images/finalMap1.png", "../resources/images/finalMap2.png", "../resources/images/finalMap3.png",
    "../resources/images/finalMap4.png","../resources/images/finalMap5.png","../resources/images/finalMap6.png","../resources/images/finalMap7.png"
    ,"../resources/images/finalMap8.png","../resources/images/finalMap9.png"});
    manager.loadMapData("../configs/finalMap"+std::to_string(1)+".json");
    Texture&texture= TexturePool::getTexture("../resources/images/girlmove.png");
    const Texture &boomLeft=TexturePool::getTexture("../resources/images/boomLeft.png");
    const Texture &boomRight=TexturePool::getTexture("../resources/images/boomRight.png");
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

    isPanelVisible = false; // 初始时不显示

    lightRadius=150.f;
    maskTexture.create(windowSize.x,windowSize.y);
    lightCircle=CircleShape(lightRadius);
    lightCircle.setOrigin(lightRadius,lightRadius);
    lightCircle.setFillColor(Color(255,255,255,0));
    lightSymmertric=CircleShape(lightRadius);
    lightSymmertric.setOrigin(lightRadius,lightRadius);
    lightSymmertric.setFillColor(Color(255,255,255,0));
    if(gameLoad==1){
        girl.loadItemQuantitiesFromFile("../configs/pack.json");
        loadFromFile("../configs/others.json");
        loadMovableObjectFromJson("../configs/player.json",girl);
        manager.returnIdx()=c_idx;
        manager.getSlimes().clear();
        manager.getRWSlimes().clear();
        manager.getIceSlimes().clear();
        manager.getspySlimes().clear();
        manager.loadMapData("../configs/finalMap"+std::to_string(c_idx+1)+".json");
    }
    
}
void LevelState::handleInput(RenderWindow& window){
    Event event;
    while(window.pollEvent(event)){
        if(event.type==Event::Closed){
            if(gameLoad==1){
                girl.saveItemQuantitiesToFile("../configs/pack.json");
                saveToFile("../configs/others.json");
                saveMovableObjectToJson("../configs/player.json",girl);
            }
            window.close();
        }

        if(event.type==Event::KeyPressed){
            if(event.key.code==Keyboard::Enter){
                game->changeState(make_unique<EndState>(game));
                c_idx=0;
            }
            return;
        }
        if(girl.toExit(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())){
            game->changeState(make_unique<EndState>(game));
            c_idx=0;
            return;
        }
        if(girl.checkwater(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())){
            game->changeState(make_unique<EndState>(game));
            c_idx=0;
            return;
        }
        if(!girl.isAlive()){
            game->changeState(make_unique<EndState>(game));
            c_idx=0;
            return;
        }
        sidebar.isClickOpener(window);
        if(sidebar.isSidebarVisible()){
            isPanelVisible = true;
        }
        else{
            isPanelVisible = false;
        }

    }
}

void LevelState::update(){
    float deltaTime=clock.restart().asSeconds();//获取帧时间差
    DELTATIME=deltaTime;
    girl.update(deltaTime,manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY(),manager.getSlimes(),manager.getIceSlimes(),manager.getRWSlimes(),manager.getspySlimes());
    manager.updateSlime(girl);
    manager.updateCoin(girl);
    if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==51){
        float gsx=20.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomLeft.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(2,girl,gsx,gsy);
    }//51 1t2
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==52){
        float gsx=bottomRight.x-48.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomRight.y-80.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(1,girl,gsx,gsy);
    }//52 2t1
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==53){
        float gsx=topRight.x-32.f*manager.getCurrentBackground().returnScaleX();
        float gsy=16.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(3,girl,gsx,gsy);
    }//53 2t3
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==54){
        float gsx=bottomRight.x-64.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomRight.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(2,girl,gsx,gsy);
    }//54 3t2
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==55){
        float gsx=bottomRight.x-45.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomRight.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(4,girl,gsx,gsy);
    }//55 3t4_1
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==56){
        float gsx=topRight.x-48.f*manager.getCurrentBackground().returnScaleX();
        float gsy=topRight.y+32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(4,girl,gsx,gsy);
    }//56 3t4_2
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==57){
        float gsx=bottomLeft.x+32.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomLeft.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(3,girl,gsx,gsy);
    }//57 4t3_1
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==58){
        float gsx=topLeft.x+32.f*manager.getCurrentBackground().returnScaleX();
        float gsy=topLeft.y+32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(3,girl,gsx,gsy);
    }//58 4t3_2
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==59){
        float gsx=bottomLeft.x+32.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomLeft.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(5,girl,gsx,gsy);
    }//59 3t5
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==60){
        float gsx=bottomRight.x-48.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomRight.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(3,girl,gsx,gsy);
    }//60 5t3
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==61){
        float gsx=bottomRight.x-64.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomRight.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(6,girl,gsx,gsy);
    }//61 5t6
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==62){
        float gsx=bottomLeft.x+32.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomLeft.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(8,girl,gsx,gsy);
    }//62 5t8
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==63){
        float gsx=topRight.x-32.f*manager.getCurrentBackground().returnScaleX();
        float gsy=topRight.y+32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(5,girl,gsx,gsy);
    }//63 6t5
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==64){
        float gsx=topLeft.x+18.f*manager.getCurrentBackground().returnScaleX();
        float gsy=topLeft.y+32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(7,girl,gsx,gsy);
    }//64 6t7
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==65){
        float gsx=topRight.x-48.f*manager.getCurrentBackground().returnScaleX();
        float gsy=topRight.y+32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(6,girl,gsx,gsy);
    }//65 7t6
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==66){
        float gsx=bottomRight.x-64.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomRight.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(9,girl,gsx,gsy);
    }//66 7t9
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==67){
        float gsx=topRight.x-32.f*manager.getCurrentBackground().returnScaleX();
        float gsy=topRight.y+32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(8,girl,gsx,gsy);
    }//67 7t8
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==68){
        float gsx=bottomRight.x-48.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomRight.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(5,girl,gsx,gsy);
    }//68 8t5
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==69){
        float gsx=bottomRight.x-64.f*manager.getCurrentBackground().returnScaleX();
        float gsy=bottomRight.y-32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(7,girl,gsx,gsy);
    }//69 8t7
    else if(girl.changeMap(manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY())==70){
        float gsx=topRight.x-32.f*manager.getCurrentBackground().returnScaleX();
        float gsy=topRight.y+32.f*manager.getCurrentBackground().returnScaleY();
        manager.switchMap(7,girl,gsx,gsy);
    }//70 9t7

    sidebar.displayBar(isPanelVisible,deltaTime);
}

void LevelState::render(RenderWindow& window){
    window.clear();
    if(manager.returnIdx()!=2&&manager.returnIdx()!=5){
        Vector2u windowSize = game->getWindow().getSize();
        sf::RectangleShape pinkBackground(sf::Vector2f(windowSize.x, windowSize.y));
        pinkBackground.setFillColor(sf::Color(38, 50, 56));
        game->getWindow().draw(pinkBackground);
    }
    manager.renderMap(window,manager.getCurrentMapData());
    girl.render(window);
    if (manager.returnIdx() == 2 || manager.returnIdx() == 5) {

    // 绘制黑色蒙版
    maskTexture.clear(sf::Color::Transparent);
    Vector2u windowSize = game->getWindow().getSize();

    sf::RectangleShape blackRect(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
    blackRect.setFillColor(sf::Color(0, 0, 0, 200));
    maskTexture.draw(blackRect);


    if (manager.returnIdx() == 5) {
        
        Vector2f symmertricPos={windowSize.x-girl.getPosition().x,girl.getPosition().y};
        lightSymmertric.setPosition(symmertricPos);
        maskTexture.draw(lightSymmertric,BlendNone);
    }

    // 设置光照圆形的位置为玩家位置
    lightCircle.setPosition(girl.getPosition());
    maskTexture.draw(lightCircle, sf::BlendNone);

    maskTexture.display();

    sf::Sprite maskSprite(maskTexture.getTexture());
    sf::RenderStates states;
    states.blendMode = sf::BlendMultiply;


    window.draw(maskSprite, states);
    }
    sidebar.render(window,girl);
    window.display();
}
