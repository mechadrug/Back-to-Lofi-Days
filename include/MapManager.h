#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <vector>
#include <string>
#include "../include/json.hpp"
#include "Map.h"
#include "Player.h"
#include "TexturePool.h"
class MapManager {
private:
    vector<Map> backgrounds;  // 存储所有背景地图
    vector<vector<Tile>> mapData;     // 存储所有地图数据
    int currentMapIndex;           // 当前激活的地图索引
    json loadingData;
    int jsonArrayWidth;
    int jsonArrayHeight;
    Texture& brokenWallTexture = TexturePool::getTexture("../resources/images/brokenWall.png");
    Sprite brokenWallSprite;

public:
    MapManager();
    void loadMapData(const string& mapConfigFile);
    void loadBackgrounds(const vector<string>& backgroundFiles);
    void switchMap(int mapIndex, MovableObject& girl,float x,float y);
    void renderMap(RenderWindow& window, const vector<vector<Tile>>& mapData);
    Map& getCurrentBackground();
    vector<vector<Tile>>& getCurrentMapData();
};

#endif
