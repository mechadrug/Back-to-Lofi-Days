#ifndef BACKPACK_H
#define BACKPACK_H

#include "../SFML/Graphics.hpp"
#include "Button.h"  // 引入 Button 类
#include "GlobalVar.h"
#include "Player.h"
class Bag {
private:
    Button bagButton;         // 背包按钮
    Button quitButton;
    RectangleShape buttonMask; //蒙一层磨砂效果
    sf::RectangleShape bagWindow;  // 背包界面的背景
    bool isBagOpen;               // 背包是否打开
    Clock clickClock;
    const float clickCooldown = 1.0f;
    Font font;
    wstring currentItemInfo; //物品信息
    vector<BagItemButton> itemButtons; // 背包中的按钮列表
public:
    Bag();
    void loadItems(MovableObject&girl);
    void update(sf::RenderWindow& window,MovableObject&girl);
    void render(sf::RenderWindow& window,MovableObject&girl);
    void handleClick(const sf::Vector2i& mousePos);
    void showItemInfo(const sf::Vector2i& mousePos, sf::RenderWindow& window);
    void openBag();
    void closeBag();
    bool isBagActive() const { return isBagOpen; }
    void setPos(float x,float y);
    void loadFont(const std::string& fontPath) {
        if (!font.loadFromFile(fontPath)) {
            std::cerr << "Failed to load font!" << std::endl;
        }
    }
    void equipItem(int ID,MovableObject&girl);
    void removeItemEffect(int ID, MovableObject& girl) {
    if (items[ID].id == "01") {  // 月牙项链 (提升攻击力)
        girl.restoreAttack();
    } else if (items[ID].id == "02") {  // 泡泡糖戒指 (提升攻击速度)
        girl.restoreAttackCooldown();
    } else if (items[ID].id == "03") {  // 鱼骨墨镜 (提升攻击力和攻速)
        girl.restoreAttack();
        girl.restoreAttackCooldown();
    } else if (items[ID].id == "04") {  // 奶龙玩偶 (提升攻击力和攻击范围)
        girl.restoreAttack();
        girl.restoreAtkRange();
    } else if (items[ID].id == "05") {  // 长号挂饰 (提升速度和攻击范围)
        girl.restoreSpeed();
        girl.restoreAtkRange();
    } else if (items[ID].id == "11") {  // 棒球帽 (提升血量上限)
        girl.restoreHealthCap();
    } else if (items[ID].id == "12") {  // 斗篷 (有概率被攻击不会掉血)
        girl.restoreInviProb();
    } else if (items[ID].id == "21") {  // 棉花糖围巾 (提升移速)
        girl.restoreSpeed();
    } else if (items[ID].id == "22") {  // 弹簧 (提升弹跳能力)
        girl.restoreJumpHeight();
    } else if (items[ID].id == "23") {  // 椰子壳 (延长隐身时间)
        girl.restoreInvisiTime();
    } else if (items[ID].id == "41" || items[ID].id == "42") {  // 小号果和竖琴 (暂不考虑)
        // 不处理暂不需要的物品
    }
}
    void handleItemSelection(RenderWindow&window,MovableObject&girl);
};

#endif