#ifndef SIDEBARMANAGER_H
#define SIDEBARMANAGER_H

#include <SFML/Graphics.hpp>
#include "Shop.h"
#include "Achievements.h"
#include "Backpack.h"  // 假设你有背包系统
#include "Player.h"
#include "GlobalVar.h"
#include "Rule.h"
class SidebarManager {
private:
    RectangleShape sidebar;  // 侧边栏的背景
    bool isSidebarOpen;          // 侧边栏是否展开
    Shop* shopSystem;            // 商店系统
    Achievements* achievementsSystem;  // 成就系统
    Bag* backpackSystem;          // 背包系统
    Rule* rulesSystem;
    void* activeSystem;          // 当前激活的子系统
    Button button; // 半透明按钮;当点击这个按钮之后,会呼出sidebar
    RectangleShape buttonMask; //蒙一层磨砂效果
    Clock clickClock;
    Clock hintClock;
    const float clickCooldown = 1.0f;
    RectangleShape playerMask; //角色信息显示磨砂效果
    string healthinfo;
    wstring currentEquipinfo;
    Texture& monesy=TexturePool::getTexture("../resources/images/money.png");

    Texture& health=TexturePool::getTexture("../resources/images/health.png");
    Texture& equipment=TexturePool::getTexture("../resources/images/equipments.png");
    Sprite moneySprite;
    Sprite healthSprite;
    Sprite equipmentSprite;

    Button hintButton;
    Texture& hint=TexturePool::getTexture("../resources/images/hint.png");
    bool hintOpen;
    RectangleShape hintMask; //提示显示磨砂效果
    Text info;

public:
    SidebarManager();
    ~SidebarManager();
    void render(sf::RenderWindow& window,MovableObject& girl);  // 渲染侧边栏及当前活跃的子系统
    void openSidebar();  // 展开侧边栏
    void closeSidebar(); // 收回侧边栏
    void isClickOpener(RenderWindow& window){
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            Vector2i mousePos = Mouse::getPosition(window);
            if(clickClock.getElapsedTime().asSeconds()<clickCooldown){
                return ;
            }
        
        if(button.isPressed(mousePos)){
            if(isSidebarVisible()){
                closeSidebar();
                gamePaused = false;  // 恢复游戏
            }else{
                openSidebar();
                gamePaused = true;  // 暂停游戏
            }
            clickClock.restart();
        }
    }
    }
    void displayBar(bool ipv,float deltaTime){
        if(ipv){
            // 侧边栏呼出 同时带动各个组件的按钮呼出
            if(sidebar.getPosition().x<0){
                float newPosX=sidebar.getPosition().x+600.f*deltaTime;
                if(newPosX>0)newPosX=0;
                sidebar.setPosition(newPosX,400.f);
                shopSystem->setPos(newPosX,400.f);
                achievementsSystem->setPos(newPosX,450.f);
                backpackSystem->setPos(newPosX,500.f);
                rulesSystem->setPos(newPosX,550.f);
            }
        }else{
            if (sidebar.getPosition().x > -50.f) {
            float newPosX = sidebar.getPosition().x - 400.f*deltaTime; // 平滑地向左移动
            if (newPosX < -50.f) newPosX = -50.f; // 限制横拉条位置
            sidebar.setPosition(newPosX, 400.f);
            shopSystem->setPos(newPosX,400.f);
            achievementsSystem->setPos(newPosX,450.f);
            backpackSystem->setPos(newPosX,500.f);
            rulesSystem->setPos(newPosX,550.f);
        }
        }
    }  
    // 是否点击呼出按钮
    bool isSidebarVisible() const { return isSidebarOpen; } // 当前的侧边栏是否展开
    
};

#endif
