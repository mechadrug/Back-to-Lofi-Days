#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include<iostream>
#include<fstream>
#include"../include/json.hpp"
#include"Player.h"
#include"Map.h"
class LevelState: public GameState{
    private:
    Game*game;
    Text message;
    Font font;
    Map background;
    MovableObject girl;
    json loading;
    int jsonArrayWidth;
    int jsonArrayHeight;
    vector<vector<Tile>> mapData;
    Clock clock;
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
