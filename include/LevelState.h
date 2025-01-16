#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include<iostream>
#include<fstream>
#include"json.hpp"
#include"Player.h"
#include"Map.h"
#include"TexturePool.h"
#include"MapManager.h"
#include"Slime.h"
#include"Shop.h"
#include"GlobalVar.h"
#include"SidebarManager.h"
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
    //bool gamePaused;//true->暂停;false->开始
    Shop shop;

    sf::RectangleShape panel; // 横拉条
    float panelWidth;         // 横拉条的宽度
    float panelHeight;        // 横拉条的高度
    bool isPanelVisible;      // 横拉条是否可见
    SidebarManager sidebar;

    RenderTexture maskTexture;
    CircleShape lightCircle;
    float lightRadius;
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
