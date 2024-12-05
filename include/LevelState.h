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
    Map background1;
    Map background2;
    Map background3;
    bool isBg1;
    bool isBg2;
    bool isBg3;
    bool changeMap;
    MovableObject girl;
    json loading;
    json loading1;
    json loading2;
    int jsonArrayWidth;
    int jsonArrayHeight;
    vector<vector<Tile>> mapData;
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
