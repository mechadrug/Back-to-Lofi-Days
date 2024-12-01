#ifndef ENDSTATE_H
#define ENDSTATE_H

#include "GameState.h"

class EndState:public GameState{
    private:
    Game*game;
    sf::Text subtitle;//字幕文本对象
    sf::Font font;//字体对象
    float subtitleYPos;//字幕的Y轴位置
    float subtitleSpeed;//字幕的移动速度
    Clock clock;     
    public:
    EndState(Game* game);
    void handleInput(RenderWindow& window) override;
    void update() override;
    void render(RenderWindow& window) override;
    const char* getName() const override {
        return "EndState";
    }
};


#endif
