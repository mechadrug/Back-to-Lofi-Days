#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include "GameState.h"

class LevelState: public GameState{
    private:
    Game*game;
    Text message;
    Font font;
    public:
    LevelState(Game* game);
    void handleInput(RenderWindow& window) override;
    void update() override;
    void render(RenderWindow& window) override;
    const char* getName() const override {
        return "LevelState";
    }
};

#endif
