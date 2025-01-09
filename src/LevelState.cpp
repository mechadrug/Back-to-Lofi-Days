#include "../include/LevelState.h"
#include "../include/EndState.h"
#include "../include/Game.h"
#include "../include/TexturePool.h"



LevelState::LevelState(Game*game): game(game),manager(),shop(){
    if (game == nullptr) {
        throw std::runtime_error("Game pointer is null in LevelState constructor!");
    }
    manager.loadBackgrounds({"../resources/images/finalMap1.png", "../resources/images/finalMap2.png", "../resources/images/finalMap3.png",
    "../resources/images/finalMap4.png","../resources/images/finalMap5.png","../resources/images/finalMap6.png","../resources/images/finalMap7.png"
    ,"../resources/images/finalMap8.png","../resources/images/finalMap9.png"});
    
    manager.loadMapData("../configs/finalMap1.json");
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
    //gamePaused=false;

    panelWidth = 50.f; // 横拉条宽度
    panelHeight = 0.5*windowSize.y; // 高度等于窗口高度
    panel.setSize(sf::Vector2f(panelWidth, panelHeight));
    panel.setFillColor(sf::Color(0, 0, 0, 200)); // 半透明黑色背景
    panel.setPosition(-panelWidth, 200.f); // 初始位置在屏幕外

    isPanelVisible = false; // 初始时不显示
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
        if(!girl.isAlive()){
            game->changeState(make_unique<EndState>(game));
            return;
        }

        sidebar.isClickOpener(window);
        if(sidebar.isSidebarVisible()){
            isPanelVisible = true;
        }
        else{
            isPanelVisible = false;
        }
        //

    }
}

void LevelState::update(){
    //bottomleft:(0,y0);bottomright:(x0,y0);topleft:(0,0);topright(x0,0)
    float deltaTime=clock.restart().asSeconds();//获取帧时间差
    girl.update(deltaTime,manager.getCurrentMapData(),16.0*manager.getCurrentBackground().returnScaleX(),16.0*manager.getCurrentBackground().returnScaleY(),manager.getSlimes());
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
    // sf::RectangleShape border(sf::Vector2f(window.getSize().x, window.getSize().y));
    // border.setFillColor(sf::Color::Transparent);  // 设置为透明填充
    // border.setOutlineThickness(100);  // 设置边框厚度
    // border.setOutlineColor(sf::Color(255, 255, 255));  // 设置边框颜色
    // window.draw(border);
    manager.renderMap(window,manager.getCurrentMapData());
    girl.render(window);
    //shop.update(window);
    //shop.render(window);
    //window.draw(panel);
    sidebar.render(window,girl);
    window.display();
}
