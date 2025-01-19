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
    Texture& treeTexture = TexturePool::getTexture("../resources/images/tree.png");
    Sprite treeSprite;
    Texture&hideStair=TexturePool::getTexture("../resources/images/snowstair.png");
    Sprite hideSprite;
    bool firstLoadSlime;
    bool firstLoadFlySlime;
    bool firstLoadIceSlime;
    bool firstLoadSpySlime;
    vector<unique_ptr<Slime>> slimes;
    Texture&slime=TexturePool::getTexture("../resources/images/Bob/slimeBob.png");
    vector<Coin> coins;
    vector<Harp> harps;
    vector<Trumpette>trumpets;
    vector<Puff>puffs;
    Texture&coin=TexturePool::getTexture("../resources/images/coin.png");
    Texture&harp=TexturePool::getTexture("../resources/images/harpe.png");
    Texture&trumpet=TexturePool::getTexture("../resources/images/trumpet.png");
    Texture&puff=TexturePool::getTexture("../resources/images/spuff.png");

    vector<unique_ptr<RandomWalkingSlime>> RWslimes;
    Texture&RWslime=TexturePool::getTexture("../resources/images/Fly/slimeFly.png");
    Texture&leftBoom=TexturePool::getTexture("../resources/images/Ice/leftBoom.png");
    Texture&rightBoom=TexturePool::getTexture("../resources/images/Ice/rightBoom.png");
    Texture&iceSlime=TexturePool::getTexture("../resources/images/Ice/iceman.png");
    Texture&Spyslime=TexturePool::getTexture("../resources/images/Fly/slimeFly.png");

    vector<unique_ptr<MissileSlime>> iceSlimes;
    vector<unique_ptr<SpySlime>> spySlimes;

    Texture&Re=TexturePool::getTexture("../resources/images/Re.png");
    Texture&Mi=TexturePool::getTexture("../resources/images/Mi.png");
    Texture&Fa=TexturePool::getTexture("../resources/images/Fa.png");
    Texture&Sol=TexturePool::getTexture("../resources/images/Sol.png");
    Texture&La=TexturePool::getTexture("../resources/images/La.png");
    Texture&Ti=TexturePool::getTexture("../resources/images/Ti.png");
    vector<Note>notes;

    Texture&cat=TexturePool::getTexture("../resources/images/cat.png");
    vector<Cat> catte;



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
    vector<unique_ptr<SpySlime>>&getspySlimes();

    void updateCoin(MovableObject&girl);
    int& returnIdx(){
        return currentMapIndex;
    }
};

#endif
