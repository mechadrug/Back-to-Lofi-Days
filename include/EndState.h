#ifndef ENDSTATE_H
#define ENDSTATE_H

#include "GameState.h"

class EndState:public GameState{
    private:
    Game*game;
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