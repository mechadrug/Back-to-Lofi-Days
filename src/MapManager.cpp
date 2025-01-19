#include "../include/MapManager.h"

MapManager::MapManager() : currentMapIndex(c_idx) {
    brokenWallSprite.setTexture(brokenWallTexture);
    iceCubeSprite.setTexture(iceCubeTexture);
    treeSprite.setTexture(treeTexture);
    hideSprite.setTexture(hideStair);
    firstLoadSlime=false;
    firstLoadFlySlime=false;
    firstLoadIceSlime=false;
    firstLoadSpySlime=false;
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
        harps.clear();
        trumpets.clear();
        puffs.clear();
        notes.clear();
        catte.clear();
        for (int y = 0; y < 30; ++y) {
            for (int x = 0; x < 40; ++x) {
                if (mapData[y][x].tileType == 20) {  // 假设 20 是金币
                    coins.emplace_back(x * 16.f*sx, y * 16.f*sy, coin);  // 创建金币并加入列表
                }
                if (mapData[y][x].tileType == 21) {
                    harps.emplace_back(x * 16.f*sx, y * 16.f*sy, harp);  // 创建harps并加入列表
                }
                if (mapData[y][x].tileType == 22) {  // 假设 20 是金币
                    trumpets.emplace_back(x * 16.f*sx, y * 16.f*sy, trumpet);  // 创建trumpet并加入列表
                }
                if (mapData[y][x].tileType == 23) {  // 假设 20 是金币
                    puffs.emplace_back(x * 16.f*sx, y * 16.f*sy, puff);  // 创建puff并加入列表
                }
                if (mapData[y][x].tileType == 40&&!noteOwnStatus[14]) {
                    notes.emplace_back(x * 16.f*sx, y * 16.f*sy, Re,2);  
                }if (mapData[y][x].tileType == 41&&!noteOwnStatus[15]) {  
                    notes.emplace_back(x * 16.f*sx, y * 16.f*sy, Mi,3);  
                }if (mapData[y][x].tileType == 42&&!noteOwnStatus[16]) { 
                    notes.emplace_back(x * 16.f*sx, y * 16.f*sy, Fa,4);  
                }if (mapData[y][x].tileType == 43&&!noteOwnStatus[17]) { 
                    notes.emplace_back(x * 16.f*sx, y * 16.f*sy, Sol,5);  
                }if (mapData[y][x].tileType == 44&&!noteOwnStatus[18]) { 
                    notes.emplace_back(x * 16.f*sx, y * 16.f*sy, La,6);
                }if (mapData[y][x].tileType == 45&&!noteOwnStatus[19]) { 
                    notes.emplace_back(x * 16.f*sx, y * 16.f*sy, Ti,7);
                }
                if(mapData[y][x].tileType==100){
                    catte.emplace_back(x * 16.f*sx, y * 16.f*sy, cat);
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
    if(!firstLoadSpySlime){
        for(int y1=0;y1<30;y1++){
            for(int x1=0;x1<40;x1++){
                if(mapData[y1][x1].tileType==94){
                    spySlimes.emplace_back(make_unique<SpySlime>(x1*16.f*sx,y1*16.f*sy,Spyslime,mapData));
                }
            }
        }
    firstLoadSpySlime=true;
    }
}

void MapManager::loadBackgrounds(const vector<string>& backgroundFiles) {
    // 加载背景地图
    for (const auto& file : backgroundFiles) {
        backgrounds.push_back(Map(file));
    }
}

void MapManager::switchMap(int mapIndex, MovableObject& girl,float x,float y) {
    if (mapIndex < 0 || mapIndex > backgrounds.size()) {
        cout<<backgrounds.size()<<endl;
        throw std::runtime_error("Invalid map index!");
    }
    currentMapIndex = mapIndex-1;
    c_idx=currentMapIndex;
    // 重新加载地图数据
    if(c_idx==3){
        audio.playSpecificMusic("birthday",true);
    }else if(c_idx==7){
        audio.playSoundEffect(SoundChannel::Monster,"push",SoundPriority::HIGH);
        audio.playRandomMusic();
    }else{
        audio.playRandomMusic();
    }
    loadMapData("../configs/finalMap" + std::to_string(mapIndex) + ".json");
    girl.changePositionBetweenMap(x,y);
    slimes.clear();
    RWslimes.clear();
    iceSlimes.clear();
    spySlimes.clear();
    float sx=backgrounds[currentMapIndex].returnScaleX();
    float sy=backgrounds[currentMapIndex].returnScaleY();
    for(int y1=0;y1<30;y1++){
        for(int x1=0;x1<40;x1++){
            if(mapData[y1][x1].tileType==91){
                slimes.emplace_back(make_unique<Slime>(x1*16.f*sx,y1*16.f*sy,slime));

            }
            if(mapData[y1][x1].tileType==92){
                RWslimes.emplace_back(make_unique<RandomWalkingSlime>(x1*16.f*sx,y1*16.f*sy,RWslime));

            }
            if(mapData[y1][x1].tileType==93){
                    iceSlimes.emplace_back(make_unique<MissileSlime>(x1*16.f*sx,y1*16.f*sy,iceSlime,leftBoom,rightBoom));
            }
            if(mapData[y1][x1].tileType==94){
                    spySlimes.emplace_back(make_unique<SpySlime>(x1*16.f*sx,y1*16.f*sy,Spyslime,mapData));
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
    for (auto& it : harps) {
            it.render(window);
        }
    for (auto& it : trumpets) {
            it.render(window);
        }
    for (auto& it : puffs) {
            it.render(window);
        }
    for(auto&it:notes){
        it.render(window);
    }
    for(auto&it:catte){
        it.render(window);
    }
    // 渲染碎墙,树元素
    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 40; x++) {
            if (mapData[y][x].tileType == 4 && mapData[y][x].isCollidable) {
                float tileX = x * 16.0f * backgrounds[currentMapIndex].returnScaleX();
                float tileY = y * 16.0f * backgrounds[currentMapIndex].returnScaleY();
                brokenWallSprite.setPosition(tileX, tileY);
                brokenWallSprite.setScale(backgrounds[currentMapIndex].returnScaleX(), backgrounds[currentMapIndex].returnScaleY());
                window.draw(brokenWallSprite);
            }
            if(mapData[y][x].tileType==83&&mapData[y][x].isCollidable){
                float tileX = x * 16.0f * backgrounds[currentMapIndex].returnScaleX();
                float tileY = y * 16.0f * backgrounds[currentMapIndex].returnScaleY();
                treeSprite.setPosition(tileX, tileY);
                treeSprite.setScale(backgrounds[currentMapIndex].returnScaleX(), backgrounds[currentMapIndex].returnScaleY());
                window.draw(treeSprite);
            }
            if(mapData[y][x].tileType==84&&map4win){
                float tileX = x * 16.0f * backgrounds[currentMapIndex].returnScaleX();
                float tileY = y * 16.0f * backgrounds[currentMapIndex].returnScaleY();
                hideSprite.setPosition(tileX, tileY);
                hideSprite.setScale(backgrounds[currentMapIndex].returnScaleX(), backgrounds[currentMapIndex].returnScaleY());
                window.draw(hideSprite);
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
    for(auto&slime:spySlimes){
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
    for(auto&slime:spySlimes){
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
vector<unique_ptr<SpySlime>>&MapManager::getspySlimes(){
    return spySlimes;
}
void MapManager::updateCoin(MovableObject&girl){
    for(auto&coin:coins){
        coin.checkCollision(girl);
    }
    for(auto&it:harps){
        it.checkCollision(girl);
    }
    for(auto&it:trumpets){
        it.checkCollision(girl);
    }
    for(auto&it:puffs){
        it.checkCollision(girl);
    }
    for(auto&it:notes){
        it.checkCollision(girl);
    }
    for(auto&it:catte){
        it.checkCollision(girl);
    }
}