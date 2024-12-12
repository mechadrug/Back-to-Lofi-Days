#include "../include/Player.h"
#include "../include/Slime.h"
MovableObject::MovableObject(float x, float y, const sf::Texture& texture,float sx,float sy)
        : position(x, y), speed(350.f),verticalSpeed(0.f),gravity(580.f),jumpHeight(533.f),isJumping(false),inertiaSpeed(0.f),Health(10),Attack(2), isDead(false),attackCooldown(1.f),lastAttackTime(chrono::steady_clock::now()),intoWind(false){
        sprite.setPosition(position);
        sprite.setTexture(texture);
        sprite.setScale(sx,sy);
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
        if(mapData[top][right].tileType==7||mapData[bottom][right].tileType==7)return 7;
        if(mapData[top][left].tileType==8||mapData[bottom][left].tileType==8)return 8;
        if(mapData[bottom][right].tileType==9||mapData[bottom][left].tileType==9)return 9;
        if(mapData[top][right].tileType==10||mapData[top][left].tileType==10)return 10;

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
        //普通墙体
        if (Keyboard::isKeyPressed(Keyboard::J)) {
        attack(slimes);  // 调用攻击方法
        }
        if(Keyboard::isKeyPressed(Keyboard::H)&& canUseInvisibility()){
            startInvisibility();
        }
        checkInvisibilityCooldowm(deltaTime);
        // if (isOnMucous) {
        //     speed *= 0.5f;
        //     jumpHeight *= 0.7f;
        // } else {
        //     speed = 350.f; // 恢复默认速度
        //     jumpHeight = 533.f; // 恢复默认跳跃高度
        // }
        if(!isOnIce){
            inertiaSpeed=0; 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (!checkCollision(position.x + speed * deltaTime, position.y, mapData, tileWidth, tileHeight,1)) {

                position.x += speed * deltaTime;//向右移动
                if(isOnMucous){
                    position.x-=0.3*speed*deltaTime;
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (!checkCollision(position.x - speed * deltaTime, position.y, mapData, tileWidth, tileHeight,2)) {
                position.x -= speed * deltaTime;//向左移动
                if(isOnMucous){
                    position.x+=0.3*speed*deltaTime;
                }
            }
        }
        }
        
            //cout<<0<<endl;
        
        if(isOnIce){//冰面
        //cout<<1<<endl;
            float ks=0.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
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
        // 风场效果
        if (isInWindLR) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                position.x += (speed * 1.2f) * deltaTime;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                position.x -= (speed * 1.2f) * deltaTime;
            }
        }
        // if (isInWindUD) {

        //     if (verticalSpeed > 0) { // 下落
        //         verticalSpeed *= 0.7f;
        //     } else if (verticalSpeed < 0) { // 上升
        //         verticalSpeed *= 1.5f;
        //     }

        // }
        // 梯子效果
        if (isOnLadder) {
            verticalSpeed = 0;
            isJumping = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                position.y -= 0.8*speed * deltaTime;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                position.y += 0.8*speed * deltaTime;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
            isJumping = true;
            verticalSpeed = -jumpHeight;//跳跃时给予一个负的初速度
            if(isOnMucous){
                verticalSpeed*=0.7f;
            }
        }
        if(isJumping){
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
                    if(isInWindUD){
                        position.y-=0.2*verticalSpeed*deltaTime;
                    }
                    position.y+=verticalSpeed*deltaTime;
                    verticalSpeed+=gravity*deltaTime;
                }
            }//降落
        }
        if(checkCollision(position.x, position.y + speed * deltaTime, mapData, tileWidth, tileHeight,4)){
            isJumping=false;
        }//停止降落
        sprite.setPosition(position);  // 更新精灵的位置
}
void MovableObject::render(RenderWindow&window){
    window.draw(sprite);
}
void MovableObject::attack(vector<Slime>& slimes){
    // 只有在攻击冷却时间过去后才能攻击
    if (chrono::duration<float>(chrono::steady_clock::now() - lastAttackTime).count() >= attackCooldown) {
        // 遍历所有史莱姆并检测攻击范围
        for (auto& slime : slimes) {
            float distanceX = abs(position.x - slime.getPosition().x);
            float distanceY = abs(position.y - slime.getPosition().y);

            // 假设攻击范围是两格（每格16像素）
            if (distanceX <= 32.0f*1.8f && distanceY <= 32.0f*1.8f && slime.isAlive()) {
                slime.takeDamage(Attack);  // 对史莱姆造成伤害
                cout << "Player attacks slime!" << endl;
            }
        }

        // 重置攻击时间
        lastAttackTime = chrono::steady_clock::now();
    }
}

void MovableObject::startInvisibility(){
    if(canUseInvisibility()){
        cout<<"girl use invisibility"<<endl;
        isInvisible=true;
        invisibilityDuration=2.0f;
        lastInvisibilityTime=chrono::steady_clock::now();
    }
}
bool MovableObject::canUseInvisibility(){
    return chrono::duration<float>(chrono::steady_clock::now()-lastInvisibilityTime).count()>=10.0f;
}

void MovableObject::checkInvisibilityCooldowm(float deltaTime){
    if(isInvisible){
        invisibilityDuration -=deltaTime;
        if(invisibilityDuration<=0){
            cout<<"invisibility goes away"<<endl;
            isInvisible=false;
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
            mapData[y][x].isCollidable=(mapData[y][x].tileType==1||mapData[y][x].tileType==2||mapData[y][x].tileType==4||mapData[y][x].tileType==9);//124可碰撞
            mapData[y][x].isBrokenable=(mapData[y][x].tileType==4);//4是碎墙,是否能够站立由碰撞检测决定
            mapData[y][x].isIce=(mapData[y][x].tileType==2);//2是冰
            mapData[y][x].isWater=(mapData[y][x].tileType==3);//3是水
            mapData[y][x].isMucous=(mapData[y][x].tileType==9);//9是粘液
            mapData[y][x].isLadder=(mapData[y][x].tileType==21);//21是梯子
            mapData[y][x].isWindLR=(mapData[y][x].tileType==22);//21是梯子
            mapData[y][x].isWindUD=(mapData[y][x].tileType==23);//21是梯子
            // mapData[y][x].from2intoMap3=(mapData[y][x].tileType==7);//map2->map3
            // mapData[y][x].from3intoMap2=(mapData[y][x].tileType==8);//map3->map2
            // mapData[y][x].from3intoMap4=(mapData[y][x].tileType==9);//map3->map4
            // mapData[y][x].from4intoMap3=(mapData[y][x].tileType==10);//map4->map3
            mapData[y][x].intoExit=(mapData[y][x].tileType==6);//map4->exit
            //mapData[y][x].isChange=(mapData[y][x].tileType==7||mapData[y][x].tileType==8||mapData[y][x].tileType==9||mapData[y][x].tileType==10);//共性
            
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

