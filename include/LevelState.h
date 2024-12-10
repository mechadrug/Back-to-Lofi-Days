#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include<iostream>
#include<fstream>
#include"json.hpp"
#include"Player.h"
#include"Map.h"
#include"TexturePool.h"
#include"MapManager.h"
class LevelState: public GameState{
    private:
    Game*game;
    Text message;
    Font font;
    MapManager manager;
    MovableObject girl;
    Clock clock;

    Vector2f topLeft;
    Vector2f topRight;
    Vector2f bottomLeft;
    Vector2f bottomRight;

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
