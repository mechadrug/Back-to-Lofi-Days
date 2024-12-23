


#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"
#include "Button.h"
#include <memory>
#include "Map.h"
using namespace std;
using namespace sf;

class MenuState: public GameState{
    private:
    Game *game;
    Button startButton;//开始按钮
    Button exitButton;//退出按钮
    sf::Font font;
    Map background;
    public:
    MenuState(Game* game);
    void handleInput(RenderWindow& window) override;
    void update() override;
    void render(RenderWindow& window) override;
     const char* getName() const override {
         return "MenuState";
     }
    
};


#endif