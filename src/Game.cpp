#include "../include/Game.h"
#include "../include/MenuState.h"

Game::Game():window(VideoMode(800,600),"game frame test"){
    //默认进入菜单状态:
    changeState(make_unique<MenuState>(this));
}//Game类的构造函数

Game::~Game(){}//使用智能指针不需要手动释放内存了太好了

void Game::run(){
    while(window.isOpen()){
        if(currentState){
            currentState->handleInput(window);
            currentState->update();
            currentState->render(window);
            //处理交互更新渲染三部曲.
        }
    }
}//游戏主循环!

void Game::changeState(unique_ptr<GameState> newState){
    currentState=move(newState);//move正好顺便清理原来的内存了
}//更新游戏关卡状态

RenderWindow& Game::getWindow(){
    return window;
}//返回一个窗口对象