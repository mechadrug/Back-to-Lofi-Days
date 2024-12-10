#include "../include/MapManager.h"

MapManager::MapManager() : currentMapIndex(0) {
    brokenWallSprite.setTexture(brokenWallTexture);
}

void MapManager::loadMapData(const std::string& mapConfigFile) {
    // 使用 json 读取地图配置文件
    loadingData = load_map(mapConfigFile);
    jsonArrayWidth = loadingData["layers"][0]["width"];
    jsonArrayHeight = loadingData["layers"][0]["height"];
    mapData = load_map_data(loadingData, jsonArrayWidth, jsonArrayHeight);
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
    currentMapIndex = mapIndex;

    // 重新加载地图数据
    loadMapData("../configs/Map" + std::to_string(mapIndex + 1) + ".json");
    girl.changePositionBetweenMap(x,y);
}

void MapManager::renderMap(RenderWindow& window, const std::vector<std::vector<Tile>>& mapData) {
    // 渲染当前地图背景
    backgrounds[currentMapIndex].draw(window);

    // 渲染碎墙等元素
    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 40; x++) {
            if (mapData[y][x].tileType == 4 && mapData[y][x].isCollidable) {
                float tileX = x * 16.0f * backgrounds[currentMapIndex].returnScaleX();
                float tileY = y * 16.0f * backgrounds[currentMapIndex].returnScaleY();
                brokenWallSprite.setPosition(tileX, tileY);
                brokenWallSprite.setScale(backgrounds[currentMapIndex].returnScaleX(), backgrounds[currentMapIndex].returnScaleY());
                window.draw(brokenWallSprite);
            }
        }
    }
}

vector<vector<Tile>>& MapManager::getCurrentMapData(){
    return mapData;
}

Map& MapManager::getCurrentBackground() {
    return backgrounds[currentMapIndex];
}