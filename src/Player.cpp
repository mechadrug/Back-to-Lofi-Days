#include "../include/Player.h"
MovableObject::MovableObject(float x, float y, const sf::Texture& texture,float sx,float sy)
        : position(x, y), speed(350.f),verticalSpeed(0.f),gravity(700.f),jumpHeight(533.f),isJumping(false) {
        sprite.setPosition(position);
        sprite.setTexture(texture);
        sprite.setScale(sx,sy);
    }
bool MovableObject::checkCollision(float newX,float newY, const vector<vector<Tile>>&mapData, int tileWidth, int tileHeight,int select){
        int left=newX/tileWidth;//角色左边所在瓦片的索引
        int right=(newX+sprite.getGlobalBounds().width)/tileWidth;//右边所在索引
        int top=newY/tileHeight;//头顶所在瓦片索引
        int bottom = (newY + sprite.getGlobalBounds().height) / tileHeight;//脚底所在瓦片索引
        cout<<left<<" "<<right<<" "<<top<<" "<<bottom<<endl;
        // 确保新的位置在地图范围内
        if (left < 0 || left >= mapData[0].size() || top <= 0 || top >= mapData.size()||right>=mapData.size()||right<=0||bottom<=0||bottom>=mapData.size()) {
            return true;  // 如果超出边界，认为发生了碰撞
        }

        // 检查该位置的瓦片是否可碰撞
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
void MovableObject::update(float deltaTime, const vector<vector<Tile>>& mapData, int tileWidth, int tileHeight){
        // 使用 deltaTime 来平滑速度
        //1:向右检测,应该检测右上和右下
        //2:向左检测,应该检测左上和左下
        //3:向上检测,应该检测左上和右上
        //4;向下检测,应该检测左下和右下
        float scaledX = position.x / sprite.getScale().x;
        float scaledY = position.y / sprite.getScale().y;
        if(!checkCollision(scaledX, scaledY + speed * deltaTime, mapData, tileWidth, tileHeight,4)){//
            position.y += (speed/2) * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (!checkCollision(scaledX + speed * deltaTime, scaledY, mapData, tileWidth, tileHeight,1)) {
                position.x += speed * deltaTime;  // 向右移动
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (!checkCollision(scaledX - speed * deltaTime, scaledY, mapData, tileWidth, tileHeight,2)) {
                position.x -= speed * deltaTime;  // 向左移动
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
            isJumping = true;
            verticalSpeed = -jumpHeight;  // 跳跃时给予一个负的初速度
        }
        if(isJumping){
            if(!checkCollision(scaledX, scaledY - (speed/2) * deltaTime, mapData, tileWidth, tileHeight,3)&&verticalSpeed<0){
                position.y+=verticalSpeed*deltaTime;
                verticalSpeed+=gravity*deltaTime;
            }//上升
            if(checkCollision(scaledX, scaledY - (speed/2) * deltaTime, mapData, tileWidth, tileHeight,3)){
                verticalSpeed=0;
            }
            if(verticalSpeed>=0){
                if(!checkCollision(scaledX, scaledY + (speed/2) * deltaTime, mapData, tileWidth, tileHeight,4)){
                    position.y+=verticalSpeed*deltaTime;
                    verticalSpeed+=gravity*deltaTime;
                }
            }//降落
        }
        if(checkCollision(scaledX, scaledY + speed * deltaTime, mapData, tileWidth, tileHeight,4)){
            isJumping=false;
        }
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
    vector<vector<Tile>> mapData(height,vector<Tile>(width));

    int index=0;
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            mapData[y][x].tileType=data[index++];
            mapData[y][x].isCollidable=(mapData[y][x].tileType==28);//28可碰撞
            mapData[y][x].intoExit=(mapData[y][x].tileType==55);//55标志着进入到结束.
        }
    }
    return mapData;
}

