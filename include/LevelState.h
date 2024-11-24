#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include "GameState.h"

class LevelState: public GameState{
    private:
    Game*game;
    public:
    LevelState(Game* game);
    void handleInput(RenderWindow& window) override;
    void update() override;
    void render(RenderWindow& window) override;
};

#endif