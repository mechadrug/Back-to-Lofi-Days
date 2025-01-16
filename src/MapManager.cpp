#include "../include/MapManager.h"

MapManager::MapManager() : currentMapIndex(0) {
    brokenWallSprite.setTexture(brokenWallTexture);
    iceCubeSprite.setTexture(iceCubeTexture);
    firstLoadSlime=false;
    firstLoadFlySlime=false;
    firstLoadIceSlime=false;
}

void MapManager::loadMapData(const std::string& mapConfigFile) {
    // 使用 json 读取地图配置文件
    loadingData = load_map(mapConfigFile);
    jsonArrayWidth = loadingData["layers"][0]["width"];
    jsonArrayHeight = loadingData["layers"][0]["height"];
    mapData = load_map_data(loadingData, jsonArrayWidth, jsonArrayHeight);
    float sx=backgrounds[0].returnScaleX();
    float sy=backgrounds[0].returnScaleY();
    // 加载金币位置
        coins.clear();  // 清空之前的金币
        for (int y = 0; y < 30; ++y) {
            for (int x = 0; x < 40; ++x) {
                if (mapData[y][x].tileType == 20) {  // 假设 20 是金币
                    coins.emplace_back(x * 16.f*sx, y * 16.f*sy, coin);  // 创建金币并加入列表
                }
            }
        }

    if(!firstLoadSlime){
        for(int y1=0;y1<30;y1++){
            for(int x1=0;x1<40;x1++){
                if(mapData[y1][x1].tileType==91){
                    slimes.emplace_back(make_unique<Slime>(x1*16.f*sx,y1*16.f*sy,slime));

                }
            }
        }
    firstLoadSlime=true;
    }
    if(!firstLoadFlySlime){
        for(int y1=0;y1<30;y1++){
            for(int x1=0;x1<40;x1++){
                if(mapData[y1][x1].tileType==92){
                    RWslimes.emplace_back(make_unique<RandomWalkingSlime>(x1*16.f*sx,y1*16.f*sy,RWslime));

                }
            }
        }
    firstLoadFlySlime=true;
    }
    if(!firstLoadIceSlime){
        for(int y1=0;y1<30;y1++){
            for(int x1=0;x1<40;x1++){
                if(mapData[y1][x1].tileType==93){
                    iceSlimes.emplace_back(make_unique<MissileSlime>(x1*16.f*sx,y1*16.f*sy,iceSlime,leftBoom,rightBoom));
                }
            }
        }
    firstLoadIceSlime=true;
    }
}

void MapManager::loadBackgrounds(const vector<string>& backgroundFiles) {
    // 加载背景地图
    for (const auto& file : backgroundFiles) {
        backgrounds.push_back(Map(file));
    }
}

void MapManager::switchMap(int mapIndex, MovableObject& girl,float x,float y) {
    if (mapIndex < 0 || mapIndex >= backgrounds.size()) {
        throw std::runtime_error("Invalid map index!");
    }
    currentMapIndex = mapIndex-1;
    c_idx=currentMapIndex;
    // 重新加载地图数据
    if(c_idx==3){
        audio.playSpecificMusic("birthday",true);
    }else{
        audio.playRandomMusic();

    }
    loadMapData("../configs/finalMap" + std::to_string(mapIndex) + ".json");
    girl.changePositionBetweenMap(x,y);
    slimes.clear();
    RWslimes.clear();
    iceSlimes.clear();
    float sx=backgrounds[currentMapIndex].returnScaleX();
    float sy=backgrounds[currentMapIndex].returnScaleY();
    for(int y1=0;y1<30;y1++){
        for(int x1=0;x1<40;x1++){
            if(mapData[y1][x1].tileType==91){
                slimes.emplace_back(make_unique<Slime>(x1*16.f*sx,y1*16.f*sy,slime));

            }
        }
    }
    for(int y1=0;y1<30;y1++){
        for(int x1=0;x1<40;x1++){
            if(mapData[y1][x1].tileType==92){
                RWslimes.emplace_back(make_unique<RandomWalkingSlime>(x1*16.f*sx,y1*16.f*sy,RWslime));

            }
        }
    }
    for(int y1=0;y1<30;y1++){
            for(int x1=0;x1<40;x1++){
                if(mapData[y1][x1].tileType==93){
                    iceSlimes.emplace_back(make_unique<MissileSlime>(x1*16.f*sx,y1*16.f*sy,iceSlime,leftBoom,rightBoom));
                }
            }
        }
}

void MapManager::renderMap(RenderWindow& window, const std::vector<std::vector<Tile>>& mapData) {
    // 渲染当前地图背景
    backgrounds[currentMapIndex].draw(window);

    // 渲染金币
        for (auto& coin : coins) {
            coin.render(window);
        }
    // 渲染碎墙等元素
    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 40; x++) {
            if (mapData[y][x].tileType == 4 && !mapData[y][x].isCollidable) {
                float tileX = x * 16.0f * backgrounds[currentMapIndex].returnScaleX();
                float tileY = y * 16.0f * backgrounds[currentMapIndex].returnScaleY();
                brokenWallSprite.setPosition(tileX, tileY);
                brokenWallSprite.setScale(backgrounds[currentMapIndex].returnScaleX(), backgrounds[currentMapIndex].returnScaleY());
                window.draw(brokenWallSprite);
            }
        }
    }
    // 渲染红色冰块
    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 40; x++) {
            if (mapData[y][x].tileType ==12 && mapData[y][x].rightPlace) {
                float tileX = x * 16.0f * backgrounds[currentMapIndex].returnScaleX();
                float tileY = y * 16.0f * backgrounds[currentMapIndex].returnScaleY();
                iceCubeSprite.setPosition(tileX, tileY);
                iceCubeSprite.setScale(backgrounds[currentMapIndex].returnScaleX(), backgrounds[currentMapIndex].returnScaleY());
                window.draw(iceCubeSprite);
            }
        }
    }
    for(auto&slime:slimes){
        slime->render(window);
    }
    for(auto&rwslime:RWslimes){
        rwslime->render(window);
    }
    for(auto&slime:iceSlimes){
        slime->render(window);
    }
}

vector<vector<Tile>>& MapManager::getCurrentMapData(){
    return mapData;
}

Map& MapManager::getCurrentBackground() {
    return backgrounds[currentMapIndex];
}

void MapManager::updateSlime(MovableObject&girl){
    for(auto&slime:slimes){
        slime->update(girl);
    }
    for(auto&rwslime:RWslimes){
        rwslime->update(girl);
    }
    for(auto&slime:iceSlimes){
        slime->update(girl);
    }
}

vector<unique_ptr<Slime>>& MapManager::getSlimes(){
    return slimes;
}
vector<unique_ptr<RandomWalkingSlime>>&MapManager::getRWSlimes(){
    return RWslimes;
}
vector<unique_ptr<MissileSlime>>&MapManager::getIceSlimes(){
    return iceSlimes;
}
void MapManager::updateCoin(MovableObject&girl){
    for(auto&coin:coins){
        coin.checkCollision(girl);
    }
}