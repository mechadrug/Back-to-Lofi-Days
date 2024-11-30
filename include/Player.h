#ifndef PLAYER_H
#define PLAYER_H

#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
using json=nlohmann::json;

struct Tile{
    int tileType;
    bool isCollidable;
    bool intoExit;
};
class MovableObject{
    private:
    Sprite sprite;
    Vector2f position;
    float speed;//横移速度
    float verticalSpeed;//垂直速度
    float gravity;//重力加速度
    float jumpHeight;//跳跃高度
    bool isJumping;//是否在跳跃当中
    
    public:
    MovableObject()=default;
    MovableObject(float x, float y, const Texture& texture,float sx,float sy);
    void update(float deltaTime, const vector<vector<Tile>>& mapData, int tileWidth, int tileHeight);
    void render(RenderWindow& window);
    bool checkCollision(float newX,float newY, const vector<vector<Tile>>&mapData, int tileWidth, int tileHeight,int select);//检测角色碰撞
};

json load_map(const string& filename);
vector<vector<Tile>> load_map_data(const json&map_data,int width,int height);

#endif