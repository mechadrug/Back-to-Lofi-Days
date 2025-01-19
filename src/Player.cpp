#include "../include/Player.h"
#include "../include/Slime.h"
#include <cmath> // 引入cmath库以使用floor和ceil函数

Boom::Boom(float x ,float y,const Texture&texture,BoomDirection direction,float maxDist,float speed):
    direction(direction),maxDist(maxDist),speed(speed),distanceTraveled(0.f){
        sprite.setTexture(texture);
        sprite.setPosition(x,y);
        startPosition=Vector2f(x,y);
}
void Boom::update(float deltaTime){
    sf::Vector2f movement(0.f, 0.f);
    if (direction == BoomDirection::Left) {
        movement.x -= speed * deltaTime;
    } else {
        movement.x += speed * deltaTime;
    }
    sprite.move(movement);
    distanceTraveled += std::abs(movement.x);
}
void Boom::render(RenderWindow&window){
    window.draw(sprite);
}
bool Boom::isExpired()const{
    return distanceTraveled>=maxDist;
}
bool Boom::checkCollisionBobSlime(unique_ptr<Slime>&slime){
    return sprite.getGlobalBounds().intersects(slime->getGlobalBounds());
}
bool Boom::checkCollisionIceSlime(unique_ptr<MissileSlime>&iceslime){
    return sprite.getGlobalBounds().intersects(iceslime->getGlobalBounds());
}
Vector2f Boom::getPosition()const{
    return sprite.getPosition();
}






MovableObject::MovableObject(float x,float y,const sf::Texture&texture,float sx,float sy,int choose):position(x,y){
    sprite.setPosition(position);
    sprite.setTexture(texture);
    sprite.setScale(sx,sy);
}
MovableObject::MovableObject(float x, float y, const sf::Texture& texture,float sx,float sy)
        : position(x, y), speed(350.f),verticalSpeed(0.f),gravity(580.f),jumpHeight(533.f),isJumping(false),
        inertiaSpeed(0.f),Health(10),Attack(2), isDead(false),attackCooldown(1.f),
        lastAttackTime(chrono::steady_clock::now()),intoWind(false),invisibilityDuration(2.0f),
        probWithoutBeingAttacked(0.f),atkRange(2),HealthCap(10),
      currentFrame(0), animationTimer(0.f), animationSpeed(0.1f), isAnimating(false), currentAction("idle"),invisibilityCooldown(5.f){
        sprite.setPosition(position);
        sprite.setTexture(texture);
        Vector2u textureSize = texture.getSize();

        float baseScaleX = 15.f / static_cast<float>(41.f);
        float baseScaleY = 15.f / static_cast<float>(41.f);
        

        float finalScaleX = baseScaleX * sx;
        float finalScaleY = baseScaleY * sy;
        sprite.setScale(finalScaleX,finalScaleY);
        sprite.setTextureRect(IntRect(0,0,41,41));
        for(int i=0;i<4;i++){
            idleFrames.emplace_back(IntRect(i*41,0,41,41));
            moveRightFrames.emplace_back(IntRect(i*41,41,41,41));
            moveLeftFrames.emplace_back(IntRect(i*41,2*41,41,41));
            invisiFrames.emplace_back(IntRect(i*41,5*41,41,41));
        }
        for(int i=0;i<2;i++){
            ladderFrames.emplace_back(IntRect(i*41,3*41,41,41));
            jumpFrames.emplace_back(IntRect(i*41,4*41,41,41));
            attackFrames.emplace_back(IntRect(i*41,7*41,41,41));
        }
        for(int i=0;i<3;i++){
        dodgeFrames.emplace_back(IntRect(i*41,6*41,41,41));
        }
        dieFrames.emplace_back(IntRect(0,8*41,41,41));
        money=1000;
        invisiTime=0.f;
        lastUseTime = std::chrono::steady_clock::now();
        //123456
        //789
        // 初始化原始值        
        originalValues.invisiTime=2.0f;// 1
        originalValues.attack = 2;// 2
        originalValues.attackCooldown=1.0f;// 3
        originalValues.health = 10;// 4
        originalValues.jumpHeight = 533.0f;// 5
        originalValues.speed = 350.0f;// 6
        originalValues.initprob=0.f;// 7
        originalValues.atkRange=2;// 8
        originalValues.healthCap=10;// 9
        
        idleTimer=0.f;
        idleSpeed=0.2f;
        moveTimer=0.f;
        moveSpeed=0.2f;
        ladderTimer=0.f;
        ladderSpeed=0.16f;
        invisiTimer=0.f;
        invisiTimer2=0.f;
        invisiSpeed=0.1f;
        atkTimer=0.f;
        atkSpeed=0.4f;


        mapFourWin=map4win;
        mapFiveWin=map5win;
        mapNineWin=map9win;
        falldownFreely=false;
        //c_idx=0;

        originalColor = sf::Color::White;

    }



bool MovableObject::checkCollision(float newX, float newY, std::vector<std::vector<Tile>>& mapData,
                                   float tileWidth, float tileHeight, int select) {
    // 精确计算瓦片索引，使用floor和ceil确保覆盖所有可能的瓦片
    int left = static_cast<int>(std::floor(newX / tileWidth));
    int right = static_cast<int>(std::ceil((newX + sprite.getGlobalBounds().width) / tileWidth)) - 1;
    int top = static_cast<int>(std::floor(newY / tileHeight));
    int bottom = static_cast<int>(std::ceil((newY + sprite.getGlobalBounds().height) / tileHeight)) - 1;
    switch(select) {
        case 1: { // 右
            const Tile* tileTopRight = getTileSafe(right, top, mapData);
            const Tile* tileBottomRight = getTileSafe(right, bottom, mapData);
            return (tileTopRight && tileTopRight->isCollidable)|| 
                   (tileBottomRight && tileBottomRight->isCollidable);
        }
        case 2: { // 左
            const Tile* tileTopLeft = getTileSafe(left, top, mapData);
            const Tile* tileBottomLeft = getTileSafe(left, bottom, mapData);
            return (tileTopLeft && tileTopLeft->isCollidable) || 
                   (tileBottomLeft && tileBottomLeft->isCollidable);
        }
        case 3: { // 上
            const Tile* tileTopLeft = getTileSafe(left, top, mapData);
            const Tile* tileTopRight = getTileSafe(right, top, mapData);
            return (tileTopLeft && tileTopLeft->isCollidable) || 
                   (tileTopRight && tileTopRight->isCollidable);
        }
        case 4: { // 下
            const Tile* tileBottomLeft = getTileSafe(left, bottom, mapData);
            const Tile* tileBottomRight = getTileSafe(right, bottom, mapData);
            return (tileBottomLeft && tileBottomLeft->isCollidable) || 
                   (tileBottomRight && tileBottomRight->isCollidable);
        }
        case 5: { // exit
            const Tile* tileTopRight = getTileSafe(right, top, mapData);
            const Tile* tileBottomRight = getTileSafe(right, bottom, mapData);
            return (tileTopRight && tileTopRight->intoExit) || 
                   (tileBottomRight && tileBottomRight->intoExit);
        }
        case 6: { // water
            // 计算中心瓦片索引
            int trueX = (left + right) / 2;
            int trueY = (top + bottom) / 2;
            const Tile* centerTile = getTileSafe(trueX, trueY, mapData);
            const Tile* tileBottomLeft = getTileSafe(left, bottom, mapData);
            const Tile* tileBottomRight = getTileSafe(right, bottom, mapData);
            const Tile* tileTopLeft = getTileSafe(left, top, mapData);
            const Tile* tileTopRight = getTileSafe(right, top, mapData);

            return (centerTile && centerTile->isWater) ||
                   (tileBottomLeft && tileBottomLeft->isWater) ||
                   (tileBottomRight && tileBottomRight->isWater) ||
                   (tileTopLeft && tileTopLeft->isWater) ||
                   (tileTopRight && tileTopRight->isWater);
        }
        case 7: { // change
            const Tile* tileBottomLeft = getTileSafe(left, bottom, mapData);
            const Tile* tileBottomRight = getTileSafe(right, bottom, mapData);
            const Tile* tileTopLeft = getTileSafe(left, top, mapData);
            const Tile* tileTopRight = getTileSafe(right, top, mapData);

            return (tileBottomLeft && tileBottomLeft->isChange) ||
                   (tileBottomRight && tileBottomRight->isChange) ||
                   (tileTopLeft && tileTopLeft->isChange) ||
                   (tileTopRight && tileTopRight->isChange);
        }
        default:
            return true; // 默认认为发生碰撞
    }
}

bool MovableObject::checkIce(float newX, float newY, std::vector<std::vector<Tile>>& mapData,
                             float tileWidth, float tileHeight) {
    // 计算瓦片索引
    int left = static_cast<int>(std::floor(newX / tileWidth));
    int right = static_cast<int>(std::ceil((newX + sprite.getGlobalBounds().width) / tileWidth)) - 1;
    int top = static_cast<int>(std::floor(newY / tileHeight));
    int bottom = static_cast<int>(std::ceil((newY + sprite.getGlobalBounds().height) / tileHeight)) - 1;

    // 检查瓦片索引是否在范围内并检测是否为冰面
    const Tile* tileBottomLeft = getTileSafe(left, bottom, mapData);
    const Tile* tileBottomRight = getTileSafe(right, bottom, mapData);

    return (tileBottomLeft && tileBottomLeft->isIce) ||
           (tileBottomRight && tileBottomRight->isIce);
}
bool MovableObject::checkwater( vector<vector<Tile>>& mapData, float tileWidth, float tileHeight){
    return checkCollision(position.x, position.y, mapData, tileWidth, tileHeight,6);
}
bool MovableObject::toExit( vector<vector<Tile>>& mapData, float tileWidth, float tileHeight){

    return checkCollision(position.x, position.y, mapData, tileWidth, tileHeight,5);
}//退出到exitstate

int MovableObject::changeMap(std::vector<std::vector<Tile>>& mapData, float tileWidth, float tileHeight){
    if(checkCollision(position.x, position.y, mapData, tileWidth, tileHeight, 7)){
        int left = static_cast<int>(std::floor(position.x / tileWidth));
        int right = static_cast<int>(std::ceil((position.x + sprite.getGlobalBounds().width) / tileWidth)) - 1;
        int top = static_cast<int>(std::floor(position.y / tileHeight));
        int bottom = static_cast<int>(std::ceil((position.y + sprite.getGlobalBounds().height) / tileHeight)) - 1;

        // 确保索引在范围内
        if (left < 0 || right >= static_cast<int>(mapData[0].size()) || top < 0 || bottom >= static_cast<int>(mapData.size())) {
            std::cerr << "changeMap: Tile indices out of bounds." << std::endl;
            return 0;
        }

        const Tile* tileTopRight = getTileSafe(right, top, mapData);
        const Tile* tileBottomRight = getTileSafe(right, bottom, mapData);
        const Tile* tileTopLeft = getTileSafe(left, top, mapData);
        const Tile* tileBottomLeft = getTileSafe(left, bottom, mapData);

        if (!tileTopRight || !tileBottomRight || !tileTopLeft || !tileBottomLeft) {
            std::cerr << "changeMap: One or more tiles are nullptr." << std::endl;
            return 0;
        }

        return std::max({tileTopRight->tileType, tileBottomRight->tileType, 
                        tileTopLeft->tileType, tileBottomLeft->tileType});
    }
    return 0;
}

void MovableObject::updateStandingTime(float newX, float newY, std::vector<std::vector<Tile>>& mapData,
                                      float tileWidth, float tileHeight) {
    int left = static_cast<int>(std::floor(newX / tileWidth));
    int right = static_cast<int>(std::ceil((newX + sprite.getGlobalBounds().width) / tileWidth)) - 1;
    int top = static_cast<int>(std::floor(newY / tileHeight));
    int bottom = static_cast<int>(std::ceil((newY + sprite.getGlobalBounds().height) / tileHeight)) - 1;

    auto accumulateTime = [&](int x, int y) {
        // 使用安全访问
        const Tile* tile = getTileSafe(x, y, mapData);
        if (tile && tile->isCollidable && tile->isBrokenable) { // 如果是碎墙
            // 增加standingTime
            mapData[y][x].standingTime += std::chrono::duration<float>(1.0f / 4800.0f);
        }
    };

    accumulateTime(left, bottom);
    accumulateTime(right, bottom); // 脚下的砖块检测

    // 检查并禁用瓦片
    auto checkAndDisableTile = [&](int x, int y) {
        const Tile* tile = getTileSafe(x, y, mapData);
        if (tile && tile->isBrokenable && tile->isCollidable) {
            if (mapData[y][x].standingTime.count() >= 1.0f) {
                mapData[y][x].isCollidable = false;
                mapData[y][x].standingTime = std::chrono::duration<float>(0.0f);
            }
        }
    };

    checkAndDisableTile(left, bottom);
    checkAndDisableTile(right, bottom);
}

void MovableObject::updateCube(float newX, float newY, std::vector<std::vector<Tile>>& mapData, float tileWidth, float tileHeight){
    int left = static_cast<int>(std::floor(newX / tileWidth));
    int right = static_cast<int>(std::ceil((newX + sprite.getGlobalBounds().width) / tileWidth)) - 1;
    int top = static_cast<int>(std::floor(newY / tileHeight));
    int bottom = static_cast<int>(std::ceil((newY + sprite.getGlobalBounds().height) / tileHeight)) - 1;

    // 确保索引在范围内
    if (left < 0 || right >= static_cast<int>(mapData[0].size()) || top < 0 || bottom >= static_cast<int>(mapData.size())) {
        std::cerr << "updateCube: Tile indices out of bounds." << std::endl;
        return;
    }

    auto it = rightCube.find({bottom, left});
    auto it2 = rightCube.find({bottom, right});

    // 检查左下方块
    const Tile* tileBottomLeft = getTileSafe(left, bottom, mapData);
    if(tileBottomLeft && tileBottomLeft->tileType == 12 && !tileBottomLeft->rightPlace){
        mapData[bottom][left].rightPlace = true; // 标记为已踩踏
        if(it != rightCube.end()){
            if(!rightCube[{bottom, left}]){
                AudioManager::getInstance().playSoundEffect(SoundChannel::System, "failToBuy", SoundPriority::MEDIUM);
                for(auto&x:rightCube){
                    int y = x.first.first;
                    int x_idx = x.first.second;
                    const Tile* tile = getTileSafe(x_idx, y, mapData);
                    if(tile){
                        mapData[y][x_idx].rightPlace = false;
                    }
                }
            }
        }
    }

    // 检查右下方块
    const Tile* tileBottomRight = getTileSafe(right, bottom, mapData);
    if(tileBottomRight && tileBottomRight->tileType == 12 && !tileBottomRight->rightPlace){
        mapData[bottom][right].rightPlace = true; // 标记为已踩踏
        if(it2 != rightCube.end()){
            if(!rightCube[{bottom, right}]){
                AudioManager::getInstance().playSoundEffect(SoundChannel::System, "failToBuy", SoundPriority::MEDIUM);
                for(auto&x:rightCube){
                    int y = x.first.first;
                    int x_idx = x.first.second;
                    const Tile* tile = getTileSafe(x_idx, y, mapData);
                    if(tile){
                        mapData[y][x_idx].rightPlace = false;
                    }
                }
            }
        }
    }

    // 检查所有相关方块是否已踩踏
    bool flag = true;
    for(auto&x:rightCube){
        if(x.second){
            int y = x.first.first;
            int x_idx = x.first.second;
            const Tile* tile = getTileSafe(x_idx, y, mapData);
            if(tile && tile->rightPlace){
                continue;
            } else {
                flag = false;
                break;
            }
        }
    }
    if(flag && !mapFourWin){
        mapFourWin = true;
        map4win=true;
        for(int i=14;i<=19;i++){
            mapData[i][2].isLadder=true;
        }
        acCheck[5]=true;
        AudioManager::getInstance().playSoundEffect(SoundChannel::System, "gameVictory", SoundPriority::MEDIUM);
    }
}
void MovableObject::updateTree(float newX, float newY, std::vector<std::vector<Tile>>& mapData, float tileWidth, float tileHeight){
    int left = static_cast<int>(std::floor(newX / tileWidth));
    int right = static_cast<int>(std::ceil((newX + sprite.getGlobalBounds().width) / tileWidth)) - 1;
    int top = static_cast<int>(std::floor(newY / tileHeight));
    int bottom = static_cast<int>(std::ceil((newY + sprite.getGlobalBounds().height) / tileHeight)) - 1;
    int mid=static_cast<int>(std::ceil((newX + 0.5*sprite.getGlobalBounds().width) / tileWidth)) - 1;
    // 确保索引在范围内
    if (left < 0 || right >= static_cast<int>(mapData[0].size()) || top < 0 || bottom >= static_cast<int>(mapData.size())) {
        std::cerr << "updateCube: Tile indices out of bounds." << std::endl;
        return;
    }

    auto it = rightTree.find({bottom, mid});

    // 检查左下方块
    const Tile* tileBottomLeft = getTileSafe(mid, bottom, mapData);
    if(tileBottomLeft && tileBottomLeft->tileType == 12 && !tileBottomLeft->rightPlace){
        mapData[bottom][left].rightPlace = true; // 标记为已踩踏
        if(it != rightTree.end()){
            if(!rightTree[{bottom, mid}]){
                AudioManager::getInstance().playSoundEffect(SoundChannel::System, "failToBuy", SoundPriority::MEDIUM);
                // resetToStartPosition();
                for(auto&x:rightTree){
                    int y = x.first.first;
                    int x_idx = x.first.second;
                    const Tile* tile = getTileSafe(x_idx, y, mapData);
                    if(tile){
                        mapData[y][x_idx].rightPlace = false;
                    }
                }
            }
        }
    }

    // 检查所有相关方块是否已踩踏
    bool flag = true;
    for(auto&x:rightTree){
        if(x.second){
            int y = x.first.first;
            int x_idx = x.first.second;
            const Tile* tile = getTileSafe(x_idx, y, mapData);
            if(tile && tile->rightPlace){
                continue;
            } else {
                flag = false;
                break;
            }
        }
    }
    if(flag && !mapNineWin){
        mapNineWin = true;
        map9win=mapNineWin;
        AudioManager::getInstance().playSoundEffect(SoundChannel::System, "gameVictory", SoundPriority::MEDIUM);
    }
}
void MovableObject::updateRome(vector<unique_ptr<Slime>>&slimes,vector<unique_ptr<MissileSlime>>&iceslimes,vector<unique_ptr<RandomWalkingSlime>>&rwslimes,std::vector<std::vector<Tile>>& mapData){
    for(auto& slime:slimes){
        if(slime->isAlive()){
            return;
        }
    }
    for(auto& slime:iceslimes){
        if(slime->isAlive()){
            return;
        }
    }
    for(auto& slime:rwslimes){
        if(slime->isAlive()){
            return;
        }
    }
    mapData[17][27].isCollidable=false;
    mapData[18][27].isCollidable=false;
    mapFiveWin=true;
    acCheck[6]=true;
    acCheck[15]=true;
    map5win=mapFiveWin;
    AudioManager::getInstance().playSoundEffect(SoundChannel::System, "gameVictory", SoundPriority::MEDIUM);

}
void MovableObject::update(float deltaTime,  vector<vector<Tile>>& mapData, float tileWidth, float tileHeight,vector<unique_ptr<Slime>>&slimes,vector<unique_ptr<MissileSlime>>&iceslimes ,vector<unique_ptr<RandomWalkingSlime>>&rwslimes,vector<unique_ptr<SpySlime>>&spyslimes){
        // 使用 deltaTime 来平滑速度
        //1:向右检测,应该检测右上和右下
        //2:向左检测,应该检测左上和左下
        //3:向上检测,应该检测左上和右上
        //4;向下检测,应该检测左下和右下
        AudioManager& audio = AudioManager::getInstance();
        bool isIdle=true;
        triggerReusableItems(slimes,iceslimes,rwslimes,spyslimes);
        if(gamePaused){
            return;
        }
    bool isOnIce = false;
    bool isOnMucous = false;
    bool isOnLadder = false;
    bool isInWindLR = false;
    bool isInWindUD = false;
    float spd=max(verticalSpeed,speed);
    // 当前角色所在瓦片索引
    int left = static_cast<int>(std::floor(position.x / tileWidth));
    int right = static_cast<int>(std::ceil((position.x + sprite.getGlobalBounds().width) / tileWidth)) - 1;
    int top = static_cast<int>(std::floor(position.y / tileHeight));
    int bottom = static_cast<int>(std::ceil((position.y + sprite.getGlobalBounds().height) / tileHeight)) - 1;

    // 定义一个安全访问瓦片的lambda
    auto safeAccess = [&](int x, int y) -> const Tile* {
        return getTileSafe(x, y, mapData);
    };

    // 检测当前瓦片的状态
    const Tile* tileBottomPlus1Left = safeAccess(left, bottom + 1);
    const Tile* tileBottomPlus1Right = safeAccess(right, bottom + 1);
    if ((tileBottomPlus1Left && tileBottomPlus1Left->isMucous) ||
        (tileBottomPlus1Right && tileBottomPlus1Right->isMucous)) {
        isOnMucous = true;
    }

    const Tile* tileBottomLeft = safeAccess(left, bottom);
    const Tile* tileBottomRight = safeAccess(right, bottom);
    if ((tileBottomLeft && tileBottomLeft->isLadder) ||
        (tileBottomRight && tileBottomRight->isLadder)) {
        isOnLadder = true;
    }

    if ((tileBottomLeft && tileBottomLeft->isWindLR) ||
        (tileBottomRight && tileBottomRight->isWindLR)) {
        isInWindLR = true;
    }

    if ((tileBottomLeft && tileBottomLeft->isWindUD) ||
        (tileBottomRight && tileBottomRight->isWindUD)) {
        isInWindUD = true;
    }
    // 检查是否在冰面上
    if (checkIce(position.x, position.y + spd * deltaTime, mapData, tileWidth, tileHeight)) {
        isOnIce = true;
    }

    // 处理特定条件下的立方体更新和站立时间
    if (c_idx == 3 && !mapFourWin) {
        updateCube(position.x, position.y + 1.001f*speed * deltaTime, mapData, tileWidth, tileHeight);
    }
    if(c_idx==8&&!mapNineWin){
        updateTree(position.x, position.y + 1.001f*speed * deltaTime, mapData, tileWidth, tileHeight);

    }
    if(c_idx==4&& !mapFiveWin){
        updateRome(slimes,iceslimes,rwslimes,mapData);
    }
    if(c_idx==8&&mapNineWin){
        for(int i=13;i<=19;i++){
            if(!noteOwnStatus[i]){
                getSevenNotesAndWin=false;
                break;
            }
            if(i==19){
                getSevenNotesAndWin=true;
            }
        }
        if(getSevenNotesAndWin){
            mapData[1][4].isCollidable=false;
        }else{
            mapData[1][4].isCollidable=true;
        }
    }
    updateStandingTime(position.x, position.y + 1.001f*speed * deltaTime, mapData, tileWidth, tileHeight);
    
    // 掉落检测
    if (!checkCollision(position.x, position.y + spd * deltaTime, mapData, tileWidth, tileHeight, 4)) {
        if(!isJumping&&!isOnLadder){
            falldownFreely=true;
        }
        if (!isOnLadder&&!falldownFreely) {
            position.y += (originalValues.speed / 10) * deltaTime;
            isJumping = true;
        }
    }else{
        falldownFreely=false;
        isJumping=false;
        verticalSpeed=0;
    }
        if (Keyboard::isKeyPressed(Keyboard::J)) {
            if(attack(slimes,iceslimes)){
                audio.playSoundEffect(SoundChannel::Player,"attack",SoundPriority::MEDIUM);
                if(!isAnimating||currentAction=="idle"||currentAction=="moveLeft"||currentAction=="moveRight"){
                currentAction="attack";
                isAnimating=true;
                int px=(left+right)/2;
                int py=(top+bottom)/2;
                int t=0;
                for(int i=-atkRange;i<=atkRange;i++){
                    if(0<=py&&py<=29&&0<=px+i&&px+i<=39){
                        if(mapData[py][px+i].tileType==91||mapData[py][px+i].tileType==93){
                        if(i<=0){
                            t=1;
                        }else{
                            t=0;
                        }
                        break;
                    }
                    }
                    
                }
                currentFrame=t;
            if(t==1){//左边发射
                const Texture& boomLeft=TexturePool::getTexture("../resources/images/boomLeft.png");
                booms.emplace_back(make_unique<Boom>(getPosition().x,getPosition().y,boomLeft,BoomDirection::Left,atkRange*16.0f*globalScaleX));
            }else{//右边
                const Texture& boomRight=TexturePool::getTexture("../resources/images/boomRight.png");
                booms.emplace_back(make_unique<Boom>(getPosition().x,getPosition().y,boomRight,BoomDirection::Right,atkRange*16.0f*globalScaleX));
            }
            }

            }
        }
        // 检测攻击后的炸弹
        for(auto it=booms.begin();it!=booms.end();){
            (*it)->update(DELTATIME);
            // 遍历所有史莱姆并检测攻击范围
            bool isBoom=false;
            for (auto& slime : slimes) {
                if(!slime)continue;
                if((*it)->checkCollisionBobSlime(slime)){
                    slime->takeDamage(Attack);
                    it=booms.erase(it);
                    isBoom=true;
                    break;
                }

            }
            if(isBoom){
                continue;
            }
            isBoom=false;
            for (auto& slime : iceslimes) {
                if(!slime)continue;
                if((*it)->checkCollisionIceSlime(slime)){
                    slime->takeDamage(Attack);
                    it=booms.erase(it);
                    isBoom=true;
                    break;
                }
            }
            if(isBoom){
                continue;
            }
            if((*it)->isExpired()){
                it=booms.erase(it);
                continue;
            }
            ++it;
        }
        // 隐身
        if(Keyboard::isKeyPressed(Keyboard::H)&& canUseInvisibility()){
            isIdle=false;
            if(!isAnimating||currentAction=="idle"||currentAction=="moveLeft"||currentAction=="moveRight"){
                currentAction="invisi";
                isAnimating=true;
                currentFrame=0;
            }
            startInvisibility();
        }
        checkInvisibilityCooldowm(deltaTime);

        if(!isOnIce){//不在冰面
            inertiaSpeed=0; 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            audio.playSoundEffect(SoundChannel::Player,"footSteps",SoundPriority::LOW);
            isIdle=false;
            if(!isAnimating){
                isAnimating=true;
                currentAction="moveRight";
                currentFrame=0;

            }
            if(isAnimating&&currentAction=="jump"){
                currentFrame=0;
                checkJumpAndMove=true;

            }
            if (!checkCollision(position.x + speed * deltaTime, position.y, mapData, tileWidth, tileHeight,1)) {
                position.x += speed * deltaTime;//向右移动
                if(isOnMucous){
                    position.x-=0.3*speed*deltaTime;
                }
                if(isJumping||falldownFreely){
                    position.x-=0.3*speed*deltaTime;
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            audio.playSoundEffect(SoundChannel::Player,"footSteps",SoundPriority::LOW);
            isIdle=false;
            if(!isAnimating){
                isAnimating=true;
                currentAction="moveLeft";
                currentFrame=0;
            }
            if(isAnimating&&currentAction=="jump"){
                currentFrame=1;
                checkJumpAndMove=true;
            }
            if (!checkCollision(position.x - speed * deltaTime, position.y, mapData, tileWidth, tileHeight,2)) {
                position.x -= speed * deltaTime;//向左移动
                if(isOnMucous){
                    position.x+=0.3*speed*deltaTime;
                }
                if(isJumping||falldownFreely){
                    position.x+=0.3*speed*deltaTime;
                }
            }
        }
        }

        if(isOnIce){//冰面
        //cout<<1<<endl;
            float ks=0.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            audio.playSoundEffect(SoundChannel::Player,"footSteps",SoundPriority::LOW);
                isIdle=false;
                if(!isAnimating){
                    isAnimating=true;
                    currentAction="moveRight";
                    currentFrame=0;
                }
                inertiaSpeed=speed;
                if (!checkCollision(position.x + 1.001f*speed* deltaTime, position.y, mapData, tileWidth, tileHeight,1)) {
                    position.x += inertiaSpeed * deltaTime;//向右移动
                    if(isOnMucous){
                    position.x-=0.3*speed*deltaTime;
                }
                }
                ks=1.0f;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            audio.playSoundEffect(SoundChannel::Player,"footSteps",SoundPriority::LOW);
                isIdle=false;
                if(!isAnimating){
                    isAnimating=true;
                    currentAction="moveLeft";
                    currentFrame=0;
                }
                inertiaSpeed=-speed;
                if (!checkCollision(position.x - 1.001f*speed * deltaTime, position.y, mapData, tileWidth, tileHeight,2)) {
                    position.x += inertiaSpeed * deltaTime;//向左移动
                    if(isOnMucous){
                    position.x+=0.3*speed*deltaTime;
                }
                }
                ks=1.0f;
            }
            if(inertiaSpeed>0){
                inertiaSpeed-=0.0001f*inertiaSpeed;
            }else if(inertiaSpeed<0){
                inertiaSpeed-=0.0001f*inertiaSpeed;
            }
            
            if(ks!=1.f&&!checkCollision(position.x + inertiaSpeed * deltaTime, position.y, mapData, tileWidth, tileHeight,1)&&!checkCollision(position.x - inertiaSpeed * deltaTime, position.y, mapData, tileWidth, tileHeight,2)){
                position.x += 0.2f*inertiaSpeed * deltaTime;
            }

        }
        // 左右风场效果
        if (isInWindLR) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                isIdle=false;
                if(!checkCollision(position.x + 0.61f*speed * deltaTime, position.y, mapData, tileWidth, tileHeight,1)){
                    position.x += (speed * 0.6f) * deltaTime;
                }
                
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                isIdle=false;
                if(!checkCollision(position.x - 0.61f*speed * deltaTime, position.y, mapData, tileWidth, tileHeight,2)){
                    position.x -= (speed * 0.6f) * deltaTime;
                }
                
            }
        }
        // 梯子效果
        if (isOnLadder) {

            verticalSpeed = 0;
            isJumping = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                isIdle=false;
                if(!isAnimating){
                    //currentFrame=0;
                    isAnimating=true;
                    currentAction="Ladder";
                }if(!checkCollision(position.x, position.y - (speed/2) * deltaTime, mapData, tileWidth, tileHeight,3)){
                    position.y -= 0.45*speed * deltaTime;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                isIdle=false;
                if(!isAnimating){
                    //currentFrame=0;
                    isAnimating=true;
                    currentAction="Ladder";
                }
                if(!checkCollision(position.x, position.y + (speed/2) * deltaTime, mapData, tileWidth, tileHeight,4)){
                    position.y += 0.45*speed * deltaTime;
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
            if(!falldownFreely){
            audio.playSoundEffect(SoundChannel::Player,"jump",SoundPriority::HIGH);

            isIdle=false;
            isJumping = true;
            verticalSpeed = -jumpHeight;//跳跃时给予一个负的初速度
            if(isOnMucous){
                verticalSpeed*=0.7f;
            }
            }
            else if(isOnLadder){
            audio.playSoundEffect(SoundChannel::Player,"jump",SoundPriority::HIGH);

            isIdle=false;
            isJumping = true;
            verticalSpeed = -0.6*jumpHeight;//跳跃时给予一个负的初速度
            }
            
        }
        if(isJumping){
            isIdle=false;
            isAnimating=true;
            currentAction="jump";
            if(!checkJumpAndMove){
                currentFrame=0;
            }
            if(checkJumpAndMove){
                checkJumpAndMove=false;
            }
            float mspd=max(speed,-1.6f*verticalSpeed);
            if(checkCollision(position.x, position.y + 1.601*verticalSpeed * deltaTime, mapData, tileWidth, tileHeight,3)){
                if(verticalSpeed>0){
                    verticalSpeed=10.f;
                }else{
                    verticalSpeed=0;
                }
            }//停止上升
            if(!checkCollision(position.x, position.y - mspd * deltaTime, mapData, tileWidth, tileHeight,3)&&verticalSpeed<0){
                
                if(isInWindUD){
                    position.y+=0.6*verticalSpeed*deltaTime;
                }
                position.y+=verticalSpeed*deltaTime;
                verticalSpeed+=1.6*gravity*deltaTime;
                
            }//上升
            
            if(verticalSpeed>=0){
                if(!checkCollision(position.x, position.y + verticalSpeed * deltaTime, mapData, tileWidth, tileHeight,4)){
                    //上下风场效果
                    
                    position.y+=verticalSpeed*deltaTime;
                    verticalSpeed+=2*gravity*deltaTime;
                    if(isInWindUD){
                        position.y-=0.2*verticalSpeed*deltaTime;
                    }
                }
            }//降落
        }
        if(falldownFreely&&!isJumping){
            currentAction="jump";
            if(!checkJumpAndMove){
                currentFrame=0;
            }
            if(checkJumpAndMove){
                checkJumpAndMove=false;
            }
            if(!checkCollision(position.x, position.y + 1.2*verticalSpeed * deltaTime, mapData, tileWidth, tileHeight,4)){
                    
                    position.y+=verticalSpeed*deltaTime;
                    verticalSpeed+=gravity*deltaTime;
                    //上下风场效果
                    if(isInWindUD){
                        position.y-=0.2*verticalSpeed*deltaTime;
                    }
            }
        }
        if(checkCollision(position.x, position.y+1.2*spd*deltaTime, mapData, tileWidth, tileHeight,4)){
            isJumping=false;
            falldownFreely=false;
            // isAnimating=false;
        }//停止降落
        if(!isJumping&&currentAction=="jump"){
            isIdle=true;
        }
        if(isIdle==true){
            if(currentAction=="invisi"&&invisiTimer2<0.39f){
                isAnimating=isAnimating;
            }else if(currentAction=="attack"&&atkTimer<0.4f){
                isAnimating=isAnimating;
            }
            else{
                isAnimating=false;
            }
        }
        if(invisiTimer2>=0.39f){
            invisiTimer2=0.f;
        }
        if(!isAnimating){

            //currentFrame=0;
            currentAction="idle";
        }
        if(isOnLadder){
            currentAction="Ladder";
            currentFrame=(currentFrame+1)%2;
        }
        updateAnimation(deltaTime);
        sprite.setPosition(position);  // 更新精灵的位置
}
void MovableObject::render(RenderWindow&window){
    if (isFlashing) {
        if (flashClock.getElapsedTime().asSeconds() < 0.2f) {
            sprite.setColor(sf::Color(255,0,0,128));
        } else {
            sprite.setColor(originalColor); 
            if (flashClock.getElapsedTime().asSeconds() >= 0.5f) {
                isFlashing = false;
            }
        }
    }
    window.draw(sprite);
    for(auto& boom:booms){
        boom->render(window);
    }
}
bool MovableObject::attack(vector<unique_ptr<Slime>>& slimes,vector<unique_ptr<MissileSlime>>&iceslimes){
    // 只有在攻击冷却时间过去后才能攻击
    if (chrono::duration<float>(chrono::steady_clock::now() - lastAttackTime).count() >= attackCooldown) {
        
        // 重置攻击时间
        lastAttackTime = chrono::steady_clock::now();
        return true;
    }else{
        return false;
    }
}

void MovableObject::startInvisibility(){
    if(canUseInvisibility()){
        cout<<"girl use invisibility"<<endl;
        isInvisible=true;
        invisiTime=invisibilityDuration;
        lastInvisibilityTime=chrono::steady_clock::now();
    }
}
bool MovableObject::canUseInvisibility(){
    return chrono::duration<float>(chrono::steady_clock::now()-lastInvisibilityTime).count()>=10.0f;
}

void MovableObject::checkInvisibilityCooldowm(float deltaTime){
    if(isInvisible){
        invisiTime -=deltaTime;
        if(invisiTime<=0){
            cout<<"invisibility goes away"<<endl;
            isInvisible=false;
        }
    }
}
void MovableObject::triggerReusableItems(vector<unique_ptr<Slime>>&slimes,vector<unique_ptr<MissileSlime>>&iceslimes,vector<unique_ptr<RandomWalkingSlime>>&rwslimes,vector<unique_ptr<SpySlime>>&spyslimes) {
    auto currentTime = std::chrono::steady_clock::now();
    // 计算上次触发物品效果和当前时间的时间差
    std::chrono::duration<float> timeElapsed = currentTime - lastUseTime;
    // 检查时间是否已经超过 1 秒
    if (timeElapsed.count() >= 1.0f) {  // 如果时间差大于等于1秒
        // 检查是否按下 R 键
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            // 遍历所有可重复使用的物品，并触发它们的效果
            for (auto& reusableItem : reusableItems) {
                 // 检查泡芙是否存在并且数量大于零
                if (items[reusableItem.second].id == "31" && items[reusableItem.second].quantity == 0) {
                    std::cout << "No more " << items[reusableItem.second].id << " to use." << std::endl;
                    continue;  // 如果泡芙数量为零，不再触发效果
                }
                if (items[reusableItem.second].id == "41" && items[reusableItem.second].quantity == 0) {
                    std::cout << "No more " << items[reusableItem.second].id << " to use." << std::endl;
                    continue;  // 如果小号果数量为零，不再触发效果
                }
                if (items[reusableItem.second].id == "42" && items[reusableItem.second].quantity == 0) {
                    std::cout << "No more " << items[reusableItem.second].id << " to use." << std::endl;
                    continue;  // 如果竖琴数量为零，不再触发效果
                }
                applyItemEffect(reusableItem.second,slimes,iceslimes,rwslimes,spyslimes);  // 应用物品效果
                items[reusableItem.second].use();  // 使用物品（减少数量）
               // reusableItem.second.quantity-=1;
            }
            cout << "Used something, current health: " << Health << endl;

            // 更新上次使用时间为当前时间
            lastUseTime = currentTime;
        }
    }
}
void MovableObject::applyItemEffect(int ID,vector<unique_ptr<Slime>>&slimes,vector<unique_ptr<MissileSlime>>&iceslimes,vector<unique_ptr<RandomWalkingSlime>>&rwslimes,vector<unique_ptr<SpySlime>>&spyslimes){
    if(items[ID].id=="31"){
        if(Health<HealthCap){
            audio.playSoundEffect(SoundChannel::System,"bloodUp",SoundPriority::HIGH);
            this->changeHealth(items[ID].extraBlood);
        }else{
            cout<<"FULL!"<<endl;
        }
    }else if(items[ID].id=="41"){
        audio.playSoundEffect(SoundChannel::System,"trumpette",SoundPriority::HIGH);
        for(auto&slime:rwslimes){
            slime->changeAlive();
        }
        // 小号果,吹了之后可以驱赶随机游走史莱姆
    }else if(items[ID].id=="42"){
        audio.playSoundEffect(SoundChannel::System,"harp",SoundPriority::HIGH);
        // 竖琴,弹了之后可以定住两秒
        for (auto& slime : slimes) {
            if(!slime)continue;
            slime->changeSleep();
        }
        for (auto& slime : iceslimes) {
            if(!slime)continue;
            slime->changeSleep();
        }
        for (auto& slime : spyslimes) {
            if(!slime)continue;
            slime->changeSleep();
        }
    }
}
void MovableObject::updateAnimation(float deltaTime){
    if(currentAction=="idle"){
        idleTimer+=frameClock.restart().asSeconds();
        if(idleTimer>=idleSpeed){
            idleTimer=0.f;
            sprite.setTextureRect(idleFrames[currentFrame]);
            currentFrame = (currentFrame + 1) % idleFrames.size();
        }
    }else if(currentAction=="moveRight"){
        moveTimer+=frameClock.restart().asSeconds();
        if(moveTimer>=moveSpeed){
            moveTimer=0.f;
            sprite.setTextureRect(moveRightFrames[currentFrame]);
            currentFrame=(currentFrame+1)%moveRightFrames.size();
        }
    }else if(currentAction=="moveLeft"){
        moveTimer+=frameClock.restart().asSeconds();
        if(moveTimer>=moveSpeed){
            moveTimer=0.f;
            sprite.setTextureRect(moveLeftFrames[currentFrame]);
            currentFrame=(currentFrame+1)%moveLeftFrames.size();
        }
    }
    else if(currentAction=="Ladder"){
        ladderTimer+=frameClock.restart().asSeconds();
        if(ladderTimer>=ladderSpeed){
            ladderTimer=0.f;
            sprite.setTextureRect(ladderFrames[currentFrame]);
            currentFrame=(currentFrame+1)%ladderFrames.size();
        }
    }else if(currentAction=="jump"){
        sprite.setTextureRect(jumpFrames[currentFrame]);
    }else if(currentAction=="invisi"){
        invisiTimer+=frameClock.restart().asSeconds();
        invisiTimer2+=frameClock.restart().asSeconds();
        sprite.setTextureRect(invisiFrames[currentFrame]);
        if(invisiTimer>=invisiSpeed){
            invisiTimer=0.f;
            
            currentFrame=(currentFrame+1);
            if(currentFrame==4){
                isAnimating=false;
            }
            
        }
    }else if(currentAction=="attack"){
        atkTimer+=frameClock.restart().asSeconds();
        sprite.setTextureRect(attackFrames[currentFrame]);
        if(atkTimer>=atkSpeed){
            atkTimer=0.f;
            isAnimating=false;
        }
    }
}
json load_map(const string& filename){
    ifstream file(filename);
    json map_data;
    file>>map_data;
    return map_data;
}

vector<vector<Tile>> load_map_data(const json&map_data,int width,int height){
    auto layers = map_data["layers"];
    auto firstlayer = layers[0];
    vector<int> data =firstlayer["data"];
    vector<vector<Tile>> mapData(30,vector<Tile>(40));

    int index=0;
    for(int y=0;y<30;y++){
        for(int x=0;x<40;x++){
            
            mapData[y][x].tileType=data[index++];
            mapData[y][x].isCollidable=(mapData[y][x].tileType==1||mapData[y][x].tileType==2||mapData[y][x].tileType==4||mapData[y][x].tileType==5||mapData[y][x].tileType==12||mapData[y][x].tileType==83||mapData[y][x].tileType==11||mapData[y][x].tileType==15);//1245可碰撞
            mapData[y][x].isBrokenable=(mapData[y][x].tileType==4||mapData[y][x].tileType==83);//4是碎墙,是否能够站立由碰撞检测决定
            mapData[y][x].isIce=(mapData[y][x].tileType==2);//2是冰
            mapData[y][x].isWater=(mapData[y][x].tileType==3||mapData[y][x].tileType==82||mapData[y][x].tileType==13);//3是水
            mapData[y][x].isMucous=(mapData[y][x].tileType==5||mapData[y][x].tileType==15);//5是粘液
            mapData[y][x].isLadder=(mapData[y][x].tileType==6||mapData[y][x].tileType==16);//6是梯子
            mapData[y][x].isWindLR=(mapData[y][x].tileType==7||mapData[y][x].tileType==81);//7是上下风场
            mapData[y][x].isWindUD=(mapData[y][x].tileType==8);//8是左右风场
            mapData[y][x].intoExit=(mapData[y][x].tileType==71);//map9->exit
            mapData[y][x].m1t2=(mapData[y][x].tileType==51);//1->2
            mapData[y][x].m2t1=(mapData[y][x].tileType==52);//1->2
            mapData[y][x].m2t3=(mapData[y][x].tileType==53);//1->2
            mapData[y][x].m3t2=(mapData[y][x].tileType==54);//1->2
            mapData[y][x].m3t4_1=(mapData[y][x].tileType==55);//1->2
            mapData[y][x].m3t4_2=(mapData[y][x].tileType==56);//1->2
            mapData[y][x].m4t3_1=(mapData[y][x].tileType==57);//1->2
            mapData[y][x].m4t3_2=(mapData[y][x].tileType==58);//1->2
            mapData[y][x].m3t5=(mapData[y][x].tileType==59);//1->2
            mapData[y][x].m5t3=(mapData[y][x].tileType==60);//1->2
            mapData[y][x].m5t6=(mapData[y][x].tileType==61);//1->2
            mapData[y][x].m5t8=(mapData[y][x].tileType==62);//1->2
            mapData[y][x].m6t5=(mapData[y][x].tileType==63);//1->2
            mapData[y][x].m6t7=(mapData[y][x].tileType==64);//1->2
            mapData[y][x].m7t6=(mapData[y][x].tileType==65);//1->2
            mapData[y][x].m7t9=(mapData[y][x].tileType==66);//1->2
            mapData[y][x].m7t8=(mapData[y][x].tileType==67);//1->2
            mapData[y][x].m8t5=(mapData[y][x].tileType==68);//1->2
            mapData[y][x].m8t7=(mapData[y][x].tileType==69);//1->2
            mapData[y][x].m9t7=(mapData[y][x].tileType==70);//1->2
            mapData[y][x].musicCube=(mapData[y][x].tileType==12);
            mapData[y][x].rightPlace=(mapData[y][x].tileType!=12);//等于12->一开始为假
            mapData[y][x].tree=(mapData[y][x].tileType==83);//编号83为树,可以踩塌
            mapData[y][x].isChange=(mapData[y][x].tileType==51||mapData[y][x].tileType==52||mapData[y][x].tileType==53||mapData[y][x].tileType==54
            ||mapData[y][x].tileType==55||mapData[y][x].tileType==56||mapData[y][x].tileType==57||mapData[y][x].tileType==58
            ||mapData[y][x].tileType==59||mapData[y][x].tileType==60||mapData[y][x].tileType==61||mapData[y][x].tileType==62
            ||mapData[y][x].tileType==63||mapData[y][x].tileType==64||mapData[y][x].tileType==65||mapData[y][x].tileType==66
            ||mapData[y][x].tileType==67||mapData[y][x].tileType==68||mapData[y][x].tileType==69||mapData[y][x].tileType==70);//共性
            
        }
    }
    return mapData;
}


