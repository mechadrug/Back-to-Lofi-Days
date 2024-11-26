#include "../include/Game.h"
#include "../include/MenuState.h"
#include<iostream>
Game::Game(){
    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned int screenHeight = desktopMode.height;
    unsigned int screenWidth = desktopMode.width;

    //计算窗口的高度为屏幕高度的0.8
    unsigned int windowHeight = static_cast<unsigned int>(screenHeight * 0.8);

    //根据4:3比例计算窗口宽度
    unsigned int windowWidth = windowHeight * 4 / 3;
    window.create(sf::VideoMode(windowWidth, windowHeight), "Lofi Days");
    //默认进入菜单状态:
    changeState(make_unique<MenuState>(this));
}//Game类的构造函数

Game::~Game(){}//使用智能指针不需要手动释放内存了太好了

void Game::run(){
    if(window.isOpen()){
        cout<<currentState->getName()<<endl;
    }
    while(window.isOpen()){
        if(currentState){
            if(currentState->getName()=="LevelState"){
                cout<<"12"<<endl;
            }
            currentState->handleInput(window);
            currentState->update();
            currentState->render(window);
            //处理交互更新渲染三部曲.
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