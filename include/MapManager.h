#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <vector>
#include <string>
#include "../include/json.hpp"
#include "Map.h"
#include "Player.h"
#include "TexturePool.h"
#include "Slime.h"
#include "Coin.h"
#include <memory>
#include "AudioManager.h"
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
    Texture& iceCubeTexture = TexturePool::getTexture("../resources/images/iceCube.png");
    Sprite iceCubeSprite;
    bool firstLoadSlime;
    bool firstLoadFlySlime;
    bool firstLoadIceSlime;

    vector<unique_ptr<Slime>> slimes;
    Texture&slime=TexturePool::getTexture("../resources/images/Bob/slimeBob.png");
    vector<Coin> coins;
    Texture&coin=TexturePool::getTexture("../resources/images/coin.png");
    vector<unique_ptr<RandomWalkingSlime>> RWslimes;
    Texture&RWslime=TexturePool::getTexture("../resources/images/Fly/slimeFly.png");
    Texture&leftBoom=TexturePool::getTexture("../resources/images/Ice/leftBoom.png");
    Texture&rightBoom=TexturePool::getTexture("../resources/images/Ice/rightBoom.png");
    Texture&iceSlime=TexturePool::getTexture("../resources/images/Ice/iceman.png");
    vector<unique_ptr<MissileSlime>> iceSlimes;


public:
    MapManager();
    void loadMapData(const string& mapConfigFile);
    void loadBackgrounds(const vector<string>& backgroundFiles);
    void switchMap(int mapIndex, MovableObject& girl,float x,float y);
    void renderMap(RenderWindow& window, const vector<vector<Tile>>& mapData);
    Map& getCurrentBackground();
    vector<vector<Tile>>& getCurrentMapData();
    void updateSlime(MovableObject&girl);
    vector<unique_ptr<Slime>>& getSlimes();
    vector<unique_ptr<RandomWalkingSlime>>&getRWSlimes();
    vector<unique_ptr<MissileSlime>>&getIceSlimes();
    void updateCoin(MovableObject&girl);
    int returnIdx(){
        return currentMapIndex;
    }
};

#endif
