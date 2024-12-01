#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include<vector>
using namespace std;
using namespace sf;

using json = nlohmann::json;

// Tile 结构体，用于表示地图中的每个瓦片
struct Tile1 {
    int tileType;      // 瓦片的类型（如 1, 27, 54）
    bool isCollidable; // 是否可以碰撞
};

// 加载地图数据
json load_map1(const std::string& filename) {
    ifstream file(filename);
    json map_data;
    file >> map_data;
    return map_data;
}

// 更新地图数据，将瓦片类型转换为 Tile 结构
vector<vector<Tile1>> load_map_data1(const json& map_data, int width, int height) {
    auto layers = map_data["layers"];
    auto firstlayer = layers[0];
    vector<int> data = firstlayer["data"];
    cout<<data.size()<<endl;
    vector<vector<Tile1>> mapData(height, vector<Tile1>(width));

    int index = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            mapData[y][x].tileType = data[index++];
            // 根据瓦片类型判断是否可碰撞
            mapData[y][x].isCollidable = (mapData[y][x].tileType == 28); // 478为可碰撞瓦片
        }
    }
    cout<<mapData[1].size()<<endl;
    return mapData;
}

class MovableObject1 {
public:
    MovableObject1(float x, float y, const sf::Texture& texture)
        : position(x, y), speed(150.f),verticalSpeed(0.f),gravity(500.f),jumpHeight(333.f),isJumping(false) {
        sprite.setPosition(position);
        sprite.setTexture(texture);
    }

    void update(float deltaTime, const vector<vector<Tile1>>& mapData, int tileWidth, int tileHeight) {
        // 使用 deltaTime 来平滑速度
        //1:向右检测,应该检测右上和右下
        //2:向左检测,应该检测左上和左下
        //3:向上检测,应该检测左上和右上
        //4;向下检测,应该检测左下和右下
        if(!checkCollision(position.x, position.y + speed * deltaTime, mapData, tileWidth, tileHeight,4)){//
            position.y += (speed/2) * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (!checkCollision(position.x + speed * deltaTime, position.y, mapData, tileWidth, tileHeight,1)) {
                position.x += speed * deltaTime;  // 向右移动
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (!checkCollision(position.x - speed * deltaTime, position.y, mapData, tileWidth, tileHeight,2)) {
                position.x -= speed * deltaTime;  // 向左移动
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
            isJumping = true;
            verticalSpeed = -jumpHeight;  // 跳跃时给予一个负的初速度
        }
        if(isJumping){
            if(!checkCollision(position.x, position.y - (speed/2) * deltaTime, mapData, tileWidth, tileHeight,3)&&verticalSpeed<0){
                position.y+=verticalSpeed*deltaTime;
                verticalSpeed+=gravity*deltaTime;
            }//上升
            if(checkCollision(position.x, position.y - (speed/2) * deltaTime, mapData, tileWidth, tileHeight,3)){
                verticalSpeed=0;
            }
            if(verticalSpeed>=0){
                if(!checkCollision(position.x, position.y + (speed/2) * deltaTime, mapData, tileWidth, tileHeight,4)){
                    position.y+=verticalSpeed*deltaTime;
                    verticalSpeed+=gravity*deltaTime;
                }
            }//降落
        }
        if(checkCollision(position.x, position.y + speed * deltaTime, mapData, tileWidth, tileHeight,4)){
            isJumping=false;
        }
        sprite.setPosition(position);  // 更新精灵的位置
    }

    void render(sf::RenderWindow& window) {
        window.draw(sprite);  // 渲染精灵
    }

private:
    sf::Sprite sprite;
    sf::Vector2f position;
    float speed;  // 控制移动的速度
    float verticalSpeed;  // 垂直速度
    const float gravity;  // 重力加速度
    const float jumpHeight;  // 跳跃高度
    bool isJumping;  // 是否在跳跃中
    // 检测是否与可碰撞瓦片发生碰撞
    bool checkCollision(float newX, float newY, const vector<vector<Tile1>>& mapData, int tileWidth, int tileHeight,int select) {
        //int left = newX / tileWidth;
        //int top = newY / tileHeight;
        int left=newX/tileWidth;//角色左边所在瓦片的索引
        int right=(newX+sprite.getGlobalBounds().width)/tileWidth;//右边所在索引
        int top=newY/tileHeight;//头顶所在瓦片索引
        int bottom = (newY + sprite.getGlobalBounds().height) / tileHeight;//脚底所在瓦片索引
        // 确保新的位置在地图范围内
        if (left < 0 || left >= mapData[0].size() || top <= 0 || top >= mapData.size()||right>=mapData[0].size()||right<=0||bottom<=0||bottom>=mapData.size()) {
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
};

int main() {
    RenderWindow window(VideoMode(640, 480), "Move Demo");

    json loading = load_map1("../configs/MapOne.json");
    int width = loading["layers"][0]["width"];
    int height = loading["layers"][0]["height"];
    vector<vector<Tile1>> mapData = load_map_data1(loading, width, height);

    Texture texture;
    if (!texture.loadFromFile("../resources/images/movetest.png")) {
        std::cerr << "Failed to load texture!" << std::endl;
        return -1;
    }

    // 加载地图纹理
    Texture mapTexture;
    if (!mapTexture.loadFromFile("../resources/images/MapOne.png")) {
        std::cerr << "Failed to load map texture!" << std::endl;
        return -1;
    }

    Sprite mapSprite;
    mapSprite.setTexture(mapTexture);
    //mapSprite.setScale(160.f / mapTexture.getSize().x, 160.f / mapTexture.getSize().y);  // 确保地图缩放适应窗口

    MovableObject1 girl(32.f, 16.f, texture);  // 设定初始位置

    Clock clock; // 用于计算 deltaTime

    while (window.isOpen()) {
        // 处理事件
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 计算每帧的时间间隔
        float deltaTime = clock.restart().asSeconds(); // 获取时间差，并重置时钟

        // 更新物体的位置
        girl.update(deltaTime, mapData, 16, 16);

        // 清除窗口并绘制物体
        window.clear(sf::Color::White);
        window.draw(mapSprite);  // 渲染地图
        girl.render(window);  // 渲染角色

        // 显示窗口内容
        window.display();
    }

    return 0;
}
