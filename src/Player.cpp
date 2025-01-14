#include "../include/Player.h"
#include "../include/Slime.h"
MovableObject::MovableObject(float x, float y, const sf::Texture& texture,float sx,float sy)
        : position(x, y), speed(350.f),verticalSpeed(0.f),gravity(580.f),jumpHeight(533.f),isJumping(false),
        inertiaSpeed(0.f),Health(10),Attack(2), isDead(false),attackCooldown(1.f),
        lastAttackTime(chrono::steady_clock::now()),intoWind(false),invisibilityDuration(2.0f),
        probWithoutBeingAttacked(0.f),atkRange(2),HealthCap(10),
      currentFrame(0), animationTimer(0.f), animationSpeed(0.1f), isAnimating(false), currentAction("idle"){
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

    }
bool MovableObject::checkCollision(float newX,float newY, vector<vector<Tile>>&mapData, float tileWidth, float tileHeight,int select){
        int left=newX/tileWidth;//角色左边所在瓦片的索引
        int right=(newX+sprite.getGlobalBounds().width)/tileWidth;//右边所在索引
        int top=newY/tileHeight;//头顶所在瓦片索引
        int bottom = (newY + sprite.getGlobalBounds().height) / tileHeight;//脚底所在瓦片索引
        if(select==5){
            return mapData[top][right].intoExit||mapData[bottom][right].intoExit;
        }//exit
        if(select==6){
            return mapData[bottom][left].isWater||mapData[bottom][right].isWater;
        }
        if(select==7){
            return mapData[bottom][left].isChange||mapData[bottom][right].isChange||mapData[top][left].isChange||mapData[top][right].isChange;
        }
        // 确保新的位置在地图范围内
        if (left < 0 || left >= mapData[0].size() || top < 0 || top >= mapData.size()||right>=mapData[0].size()||right<0||bottom<0||bottom>=mapData.size()) {
            return true; //如果超出边界,认为发生了碰撞
        }

        //检查该位置的瓦片是否可碰撞
        if(select==1){//right
            return mapData[top][right].isCollidable||mapData[bottom][right].isCollidable;
        }
        if(select==2){//left
            return mapData[top][left].isCollidable||mapData[bottom][left].isCollidable;
        }
        if(select==3){//up
            return mapData[top][left].isCollidable||mapData[top][right].isCollidable;
        }
        if(select==4){
            return mapData[bottom][left].isCollidable||mapData[bottom][right].isCollidable;
        }//down
        
        return true;
}
bool MovableObject::checkIce(float newX,float newY, vector<vector<Tile>>&mapData, float tileWidth, float tileHeight){
    int left=newX/tileWidth;//角色左边所在瓦片的索引
    int right=(newX+sprite.getGlobalBounds().width)/tileWidth;//右边所在索引
    int top=newY/tileHeight;//头顶所在瓦片索引
    int bottom = (newY + sprite.getGlobalBounds().height) / tileHeight;//脚底所在瓦片索引
    return mapData[bottom][left].isIce||mapData[bottom][right].isIce;
}
bool MovableObject::checkwater( vector<vector<Tile>>& mapData, float tileWidth, float tileHeight){
    return checkCollision(position.x, position.y, mapData, tileWidth, tileHeight,6);
}
bool MovableObject::toExit( vector<vector<Tile>>& mapData, float tileWidth, float tileHeight){

    return checkCollision(position.x, position.y, mapData, tileWidth, tileHeight,5);
}//退出到exitstate
int MovableObject::changeMap(vector<vector<Tile>>& mapData, float tileWidth, float tileHeight){
    if(checkCollision(position.x,position.y,mapData,tileWidth,tileHeight,7)){
        int left=position.x/tileWidth;
        int right=(position.x+sprite.getGlobalBounds().width)/tileWidth;
        int top=position.y/tileHeight;
        int bottom=(position.y+sprite.getGlobalBounds().height)/tileHeight;
        return max(max(mapData[top][right].tileType,mapData[bottom][right].tileType),max(mapData[top][left].tileType,mapData[bottom][left].tileType));
    }
    return 0;
    //2->3:7
    //3->2:8
    //3->4:9
    //4->3:10
    //4->exit:11(x)

}
void MovableObject::updateStandingTime(float newX,float newY,vector<std::vector<Tile>>& mapData,float tileWidth, float tileHeight){
    int left=newX/tileWidth;//角色左边所在瓦片的索引
    int right=(newX+sprite.getGlobalBounds().width)/tileWidth;//右边所在索引
    int top=newY/tileHeight;//头顶所在瓦片索引
    int bottom = (newY + sprite.getGlobalBounds().height) / tileHeight;//脚底所在瓦片索引
    auto accumulateTime = [&](int x,int y){
        if(mapData[y][x].isCollidable&&mapData[y][x].isBrokenable){//如果是碎墙
            mapData[y][x].standingTime+=chrono::duration<float>(1.0f/4800.0f);//taikuaile
        }
    };
    accumulateTime(left,bottom);
    accumulateTime(right,bottom);//脚下的砖块检测
    if(mapData[bottom][left].isBrokenable&&mapData[bottom][left].isCollidable){
        if(mapData[bottom][left].standingTime.count()>=1.0f){
            mapData[bottom][left].isCollidable=false;
        }
    }
    if(mapData[bottom][right].isBrokenable&&mapData[bottom][right].isCollidable){
        if(mapData[bottom][right].standingTime.count()>=1.0f){
            mapData[bottom][right].isCollidable=false;
        }
    }
}

void MovableObject::update(float deltaTime,  vector<vector<Tile>>& mapData, float tileWidth, float tileHeight,vector<Slime>&slimes){
        // 使用 deltaTime 来平滑速度
        //1:向右检测,应该检测右上和右下
        //2:向左检测,应该检测左上和左下
        //3:向上检测,应该检测左上和右上
        //4;向下检测,应该检测左下和右下
        bool isIdle=true;
        triggerReusableItems();
        if(gamePaused){
            return;
        }

        bool isOnIce = false;
        bool isOnMucous = false;
        bool isOnLadder = false;
        bool isInWindLR = false;
        bool isInWindUD = false;
        int left = position.x / tileWidth;
        int right = (position.x + sprite.getGlobalBounds().width) / tileWidth;
        int top = position.y / tileHeight;
        int bottom = (position.y + sprite.getGlobalBounds().height) / tileHeight;

        if (mapData[bottom+1][left].isMucous || mapData[bottom+1][right].isMucous) {
            isOnMucous = true;
        }
        if (mapData[bottom][left].isLadder || mapData[bottom][right].isLadder) {
            isOnLadder = true;
        }
        if (mapData[bottom][left].isWindLR || mapData[bottom][right].isWindLR) {
            isInWindLR = true;
        }
        if (mapData[bottom][left].isWindUD || mapData[bottom][right].isWindUD) {
            isInWindUD = true;
        }
        if (checkIce(position.x, position.y+ speed * deltaTime, mapData, tileWidth, tileHeight)) {
            isOnIce = true;
        }

        updateStandingTime(position.x,position.y+speed*deltaTime,mapData,tileWidth,tileHeight);
        if(!checkCollision(position.x, position.y + speed * deltaTime, mapData, tileWidth, tileHeight,4)){
            if (!isOnLadder) {
            position.y += (speed / 2) * deltaTime;
        }
        }//检测掉落
        if (Keyboard::isKeyPressed(Keyboard::J)) {
            if(attack(slimes)){
                if(!isAnimating||currentAction=="idle"||currentAction=="moveLeft"||currentAction=="moveRight"){
                currentAction="attack";
                isAnimating=true;
                int px=(left+right)/2;
                int py=(top+bottom)/2;
                int t=0;
                for(int i=-atkRange;i<=atkRange;i++){
                    if(mapData[py][px+i].tileType==91){
                        if(i<=0){
                            t=1;
                        }else{
                            t=0;
                        }
                        break;
                    }
                }
                currentFrame=t;
            }
            }
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
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
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
            }
        }
        }

        if(isOnIce){//冰面
        //cout<<1<<endl;
            float ks=0.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                isIdle=false;
                if(!isAnimating){
                    isAnimating=true;
                    currentAction="moveRight";
                    currentFrame=0;
                }
                inertiaSpeed=speed;
                if (!checkCollision(position.x + inertiaSpeed * deltaTime, position.y, mapData, tileWidth, tileHeight,1)) {
                    position.x += inertiaSpeed * deltaTime;//向右移动
                    if(isOnMucous){
                    position.x-=0.3*speed*deltaTime;
                }
                }
                ks=1.0f;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                isIdle=false;
                if(!isAnimating){
                    isAnimating=true;
                    currentAction="moveLeft";
                    currentFrame=0;
                }
                inertiaSpeed=-speed;
                if (!checkCollision(position.x - inertiaSpeed * deltaTime, position.y, mapData, tileWidth, tileHeight,2)) {
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
            
            if(ks!=1.f&&!checkCollision(position.x + speed * deltaTime, position.y, mapData, tileWidth, tileHeight,1)&&!checkCollision(position.x - inertiaSpeed * deltaTime, position.y, mapData, tileWidth, tileHeight,2)){
                position.x += 0.2f*inertiaSpeed * deltaTime;
            }

        }
        // 左右风场效果
        if (isInWindLR) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                isIdle=false;
                position.x += (speed * 1.2f) * deltaTime;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                isIdle=false;
                position.x -= (speed * 1.2f) * deltaTime;
            }
        }
        // 梯子效果
        if (isOnLadder) {

            verticalSpeed = 0;
            isJumping = false;
            bool flag=true;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                isIdle=false;
                if(!isAnimating){
                    currentFrame=0;
                    isAnimating=true;
                    currentAction="Ladder";
                }
                position.y -= 0.8*speed * deltaTime;
                flag=false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                isIdle=false;
                if(!isAnimating){
                    currentFrame=0;
                    isAnimating=true;
                    currentAction="Ladder";
                }
                position.y += 0.8*speed * deltaTime;
                flag=false;
            }
            if(flag){
                isAnimating=false;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
            isIdle=false;
            isJumping = true;
            verticalSpeed = -jumpHeight;//跳跃时给予一个负的初速度
            if(isOnMucous){
                verticalSpeed*=0.7f;
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
            if(!checkCollision(position.x, position.y - (speed/2) * deltaTime, mapData, tileWidth, tileHeight,3)&&verticalSpeed<0){
                if(isInWindUD){
                    position.y+=verticalSpeed*deltaTime;
                }
                position.y+=verticalSpeed*deltaTime;
                verticalSpeed+=gravity*deltaTime;
            }//上升
            if(checkCollision(position.x, position.y - (speed/2) * deltaTime, mapData, tileWidth, tileHeight,3)){
                verticalSpeed=0;
            }//停止上升
            if(verticalSpeed>=0){
                if(!checkCollision(position.x, position.y + (speed/2) * deltaTime, mapData, tileWidth, tileHeight,4)){
                    //上下风场效果
                    if(isInWindUD){
                        position.y-=0.2*verticalSpeed*deltaTime;
                    }
                    position.y+=verticalSpeed*deltaTime;
                    verticalSpeed+=gravity*deltaTime;
                }
            }//降落
        }
        if(checkCollision(position.x, position.y+0.8*speed*deltaTime, mapData, tileWidth, tileHeight,4)){
            isJumping=false;
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

            currentFrame=0;
            currentAction="idle";
        }
        updateAnimation(deltaTime);
        sprite.setPosition(position);  // 更新精灵的位置
}
void MovableObject::render(RenderWindow&window){
    window.draw(sprite);
}
bool MovableObject::attack(vector<Slime>& slimes){
    // 只有在攻击冷却时间过去后才能攻击
    if (chrono::duration<float>(chrono::steady_clock::now() - lastAttackTime).count() >= attackCooldown) {
        // 遍历所有史莱姆并检测攻击范围
        for (auto& slime : slimes) {
            float distanceX = abs(position.x - slime.getPosition().x);
            float distanceY = abs(position.y - slime.getPosition().y);

            // 假设攻击范围是两格（每格16像素）
            // 这里后期的代码修复需要把scaleXY改成全局的
            if (distanceX <= atkRange*16.0f*1.8f && distanceY <= atkRange*16.0f*1.8f && slime.isAlive()) {
                slime.takeDamage(Attack);  // 对史莱姆造成伤害
                cout << "Player attacks slime!" << endl;
            }
        }

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
void MovableObject::triggerReusableItems() {
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
                applyItemEffect(reusableItem.second);  // 应用物品效果
                items[reusableItem.second].use();  // 使用物品（减少数量）
               // reusableItem.second.quantity-=1;
            }
            cout << "Used something, current health: " << Health << endl;

            // 更新上次使用时间为当前时间
            lastUseTime = currentTime;
        }
    }
}
void MovableObject::applyItemEffect(int ID){
    if(items[ID].id=="31"){
        if(Health<HealthCap){
            this->changeHealth(items[ID].extraBlood);
        }else{
            cout<<"FULL!"<<endl;
        }
    }else if(items[ID].id=="41"){
        // 暂时不处理
    }else if(items[ID].id=="42"){
        // 暂时不处理
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
            mapData[y][x].isCollidable=(mapData[y][x].tileType==1||mapData[y][x].tileType==2||mapData[y][x].tileType==4||mapData[y][x].tileType==5);//1245可碰撞
            mapData[y][x].isBrokenable=(mapData[y][x].tileType==4);//4是碎墙,是否能够站立由碰撞检测决定
            mapData[y][x].isIce=(mapData[y][x].tileType==2);//2是冰
            mapData[y][x].isWater=(mapData[y][x].tileType==3);//3是水
            mapData[y][x].isMucous=(mapData[y][x].tileType==5);//5是粘液
            mapData[y][x].isLadder=(mapData[y][x].tileType==6);//6是梯子
            mapData[y][x].isWindLR=(mapData[y][x].tileType==7);//7是上下风场
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


            mapData[y][x].isChange=(mapData[y][x].tileType==51||mapData[y][x].tileType==52||mapData[y][x].tileType==53||mapData[y][x].tileType==54
            ||mapData[y][x].tileType==55||mapData[y][x].tileType==56||mapData[y][x].tileType==57||mapData[y][x].tileType==58
            ||mapData[y][x].tileType==59||mapData[y][x].tileType==60||mapData[y][x].tileType==61||mapData[y][x].tileType==62
            ||mapData[y][x].tileType==63||mapData[y][x].tileType==64||mapData[y][x].tileType==65||mapData[y][x].tileType==66
            ||mapData[y][x].tileType==67||mapData[y][x].tileType==68||mapData[y][x].tileType==69||mapData[y][x].tileType==70);//共性
            
        }
    }
    return mapData;
}
//->x
//|
//y
//0000
//0010->(1,2)1*16.0*scaley,2*16.0*scalex
//0100->(2,1)
//0000

