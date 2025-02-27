#include "../include/Game.h"
#include "../include/MenuState.h"
#include "../include/GlobalVar.h"
#include<iostream>
Game::Game(){
    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned int screenHeight = desktopMode.height;
    unsigned int screenWidth = desktopMode.width;
    Image icon;
    icon.loadFromFile("../resources/images/playMusic.png");
    //计算窗口的高度为屏幕高度的0.8
    unsigned int windowHeight = static_cast<unsigned int>(screenHeight * 0.8);
    
    //根据4:3比例计算窗口宽度
    unsigned int windowWidth = windowHeight * 4 / 3;
    window.create(sf::VideoMode(windowWidth, windowHeight), L"低保真世界");
     window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    //默认进入菜单状态:
    changeState(make_unique<MenuState>(this));
    fonte.loadFromFile("../resources/fonts/Silver.ttf");

}//Game类的构造函数

Game::~Game(){}//使用智能指针不需要手动释放内存了太好了

void Game::run(){
    while(window.isOpen()){
        if(currentState){
            currentState->handleInput(window);
            currentState->update();
            currentState->render(window);
            //处理交互更新渲染三部曲.
            AudioManager::getInstance().update();
        }
    }
}//游戏主循环!

void Game::changeState(unique_ptr<GameState> newState){
    currentState = move(newState);
    cout << "Changing state to: " << currentState->getName() << endl;  // 打印更新后的状态
}//更新游戏关卡状态

RenderWindow& Game::getWindow(){
    return window;
}//返回一个窗口对象