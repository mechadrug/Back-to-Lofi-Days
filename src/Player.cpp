#include "../include/Player.h"
MovableObject::MovableObject(float x, float y, const sf::Texture& texture,float sx,float sy)
        : position(x, y), speed(350.f),verticalSpeed(0.f),gravity(700.f),jumpHeight(533.f),isJumping(false),inertiaSpeed(0.f) {
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

void MovableObject::update(float deltaTime,  vector<vector<Tile>>& mapData, float tileWidth, float tileHeight){
        // 使用 deltaTime 来平滑速度
        //1:向右检测,应该检测右上和右下
        //2:向左检测,应该检测左上和左下
        //3:向上检测,应该检测左上和右上
        //4;向下检测,应该检测左下和右下
        bool isOnIce = false;
        if (checkIce(position.x, position.y+ speed * deltaTime, mapData, tileWidth, tileHeight)) {
            isOnIce = true;
        }

        updateStandingTime(position.x,position.y+speed*deltaTime,mapData,tileWidth,tileHeight);
        if(!checkCollision(position.x, position.y + speed * deltaTime, mapData, tileWidth, tileHeight,4)){
            position.y += (speed/2) * deltaTime;
        }//检测掉落
        //普通墙体
        if(!isOnIce){
            inertiaSpeed=0; 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (!checkCollision(position.x + speed * deltaTime, position.y, mapData, tileWidth, tileHeight,1)) {
                position.x += speed * deltaTime;//向右移动
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (!checkCollision(position.x - speed * deltaTime, position.y, mapData, tileWidth, tileHeight,2)) {
                position.x -= speed * deltaTime;//向左移动
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
                }
                ks=1.0f;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                inertiaSpeed=-speed;
                if (!checkCollision(position.x - inertiaSpeed * deltaTime, position.y, mapData, tileWidth, tileHeight,2)) {
                    position.x += inertiaSpeed * deltaTime;//向左移动
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
            isJumping = true;
            verticalSpeed = -jumpHeight;//跳跃时给予一个负的初速度
        }
        if(isJumping){
            if(!checkCollision(position.x, position.y - (speed/2) * deltaTime, mapData, tileWidth, tileHeight,3)&&verticalSpeed<0){
                position.y+=verticalSpeed*deltaTime;
                verticalSpeed+=gravity*deltaTime;
            }//上升
            if(checkCollision(position.x, position.y - (speed/2) * deltaTime, mapData, tileWidth, tileHeight,3)){
                verticalSpeed=0;
            }//停止上升
            if(verticalSpeed>=0){
                if(!checkCollision(position.x, position.y + (speed/2) * deltaTime, mapData, tileWidth, tileHeight,4)){
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
            mapData[y][x].isCollidable=(mapData[y][x].tileType==1||mapData[y][x].tileType==2||mapData[y][x].tileType==3||mapData[y][x].tileType==4);//1234可碰撞
            mapData[y][x].isBrokenable=(mapData[y][x].tileType==4);//4是碎墙,是否能够站立由碰撞检测决定
            mapData[y][x].isIce=(mapData[y][x].tileType==3);//3是冰
            mapData[y][x].isWater=(mapData[y][x].tileType==5);//5是水
            mapData[y][x].from2intoMap3=(mapData[y][x].tileType==7);//map2->map3
            mapData[y][x].from3intoMap2=(mapData[y][x].tileType==8);//map3->map2
            mapData[y][x].from3intoMap4=(mapData[y][x].tileType==9);//map3->map4
            mapData[y][x].from4intoMap3=(mapData[y][x].tileType==10);//map4->map3
            mapData[y][x].intoExit=(mapData[y][x].tileType==11);//map4->exit
            mapData[y][x].isChange=(mapData[y][x].tileType==7||mapData[y][x].tileType==8||mapData[y][x].tileType==9||mapData[y][x].tileType==10);//共性
            
        }
    }
    return mapData;
}

