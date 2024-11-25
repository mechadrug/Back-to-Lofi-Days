#ifndef GAME_H
#define GAME_H


#include<SFML/Graphics.hpp>
#include<memory>
#include"GameState.h"
using namespace std;
using namespace sf;
class Game{
    private:
    RenderWindow window;
    unique_ptr<GameState> currentState;
    View view;//用于缩放
    Vector2u baseResolution= {800,600};//基础分辨率

    public:
    Game();
    ~Game();

    void run();//游戏主循环
    void changeState(unique_ptr<GameState> newState);//切换状态
    RenderWindow& getWindow();//获取窗口对象
};

#endif
//320*3=960,540