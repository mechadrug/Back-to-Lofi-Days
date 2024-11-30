#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

using json = nlohmann::json;

json load_map(const std::string& filename){
    ifstream file(filename);
    json map_data;
    file >> map_data;
    return map_data;
}

class Girl{
    private:
    Sprite sprite;
    Vector2f position;//位置
    Vector2f velocity;//速度
    bool onGround;//是否站在地面上
    bool canJump;//是否可以跳跃
    bool canMoveLeft;//是否可以向左移动
    bool canMoveRight;//是否可以向右移动
    float gravity;//重力加速度
    float jumpstrength;//跳跃力度
    float speed;//行动速度
    //float frameTime;//帧速度
    //int currentFrame;//当前帧
    int spriteWidth;//精灵宽
    int spriteHeight;//精灵高
    Clock clock;//记录时间
    

    
    public:
    Girl(const Texture& texture,float x,float y,int width,int height): sprite(texture), position(x, y), velocity(0.f, 0.f), onGround(true),canJump(true),canMoveLeft(false),
    canMoveRight(true),gravity(98.f),jumpstrength(-100.0f),speed(100.f),spriteWidth(width),spriteHeight(height){
        sprite.setPosition(position);
        sprite.setTexture(texture);
        sprite.setTextureRect(IntRect(0,0,width,height));

    }//构造函数
    void handleCollison(const vector<vector<int>>&mapdata,int tileWidth,int tileHeight){
        int left=position.x/tileWidth;//角色左边所在瓦片的索引
        int right=(position.x+sprite.getGlobalBounds().width)/tileWidth;//右边所在索引
        int top=position.y/tileHeight;//头顶所在瓦片索引
        int bottom = (position.y + sprite.getGlobalBounds().height) / tileHeight;//脚底所在瓦片索引
        
        onGround = false;
        canJump = true;
        canMoveLeft = true;
        canMoveRight = true;
        if(mapdata[top][left]==27 || mapdata[bottom][right]==27){
            position.y=bottom*tileHeight-sprite.getGlobalBounds().height;
            velocity.y=0;
            onGround = true;            
        }else{
            velocity.y+=gravity*0.016f;
        }
        if(mapdata[top][left]==27||mapdata[top][right]==27){
            canJump=false;
        }
          if (mapdata[top][left] == 27 || mapdata[bottom][left] == 27) {
            canMoveLeft = false;  // 左边有墙壁，不能左移
        }
        if (mapdata[top][right] == 27 || mapdata[bottom][right] == 27) {
            canMoveRight = false;  // 右边有墙壁，不能右移
        }
    }
    void update(vector<vector<int>>&mapdata,int tileWidth,int tileHeight,float deltaTime){
        handleCollison(mapdata,tileWidth,tileHeight);
        position+=velocity*deltaTime;
        sprite.setPosition(position);
    }//更新女孩的位置
    void jump(){
        if(onGround&&canJump){
            velocity.y=jumpstrength;
            onGround = false;
        }
    }//跳跃
    void moveLeft(){
         if(canMoveLeft){
            velocity.x=-speed;
         }else{
            velocity.x=0.f;
         }
    }//向左移动
    void moveRight(){
        if(canMoveRight){
            velocity.x=speed;
        }else{
            velocity.x=0.f;
        }
    }
    void render(RenderWindow&window){
        window.draw(sprite);
    }
};
int main() {
    RenderWindow window(VideoMode(160, 160), "Move Demo");

    json loading=load_map("../configs/testmap.json");

    auto layers=loading["layers"];
    auto firstlayer=layers[0];
    int width=firstlayer["width"];
    int height=firstlayer["height"];
    // 定义一个 10x10 的二维数组
    vector<int>data =firstlayer["data"];
    vector<vector<int>> mapData(10, vector<int>(10, 1));
    int index = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            mapData[i][j] = data[index++];
        }
    }
    // 加载贴图
    Texture texture;
    if (!texture.loadFromFile("../resources/images/movetest.png")) {
        std::cerr << "Failed to load texture!" << std::endl;
        return -1;
    }

    // 创建 Girl 对象
    Girl girl(texture, 100.f, 0.f,16,16); // 假设精灵大小是 32x32

    Clock clock;  // 用于计算 deltaTime
    Texture map;
    if (!texture.loadFromFile("../resources/images/fckmap.png")) {
        std::cerr << "Failed to load texture!" << std::endl;
        return -1;
    }
    Sprite mapt;
    mapt.setTexture(map);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // 获取 deltaTime
        float deltaTime = clock.restart().asSeconds();

        // 控制角色移动
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            girl.moveLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            girl.moveRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            girl.jump();
        }

        // 更新角色状态
        girl.update(mapData, 16, 16, deltaTime);

        // 清除窗口，渲染角色
        window.clear();
        //window.draw(mapt);
        girl.render(window);
        window.display();
    }

    return 0;
}
