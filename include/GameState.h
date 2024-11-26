#ifndef GAMESTATE_H
#define GAMESTATE_H



#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Game;//前置声明避免循环依赖

class GameState{//游戏关卡虚基类
    public:
    virtual ~GameState()=default;

    virtual void handleInput(sf::RenderWindow &window) = 0;
    virtual void update() = 0;
    virtual void render(sf::RenderWindow &window) = 0;
    virtual const char* getName() const = 0;
};

#endif
