#ifndef BUTTON_H
#define BUTTON_H

#include"GlobalVar.h"
#include <SFML/Graphics.hpp>
#include <string>
#include<memory>
#include "../include/TexturePool.h"
#include "Item.h"
using namespace std;
class Button {
public:
    // 构造函数
    Button()=default;
    Button(float x, float y, float width, float height, const std::string& textureFile);
    // 渲染按钮
    void draw(sf::RenderWindow& window);
    // 检查鼠标点击是否在按钮区域内
    bool isPressed(sf::Vector2i mousePosition);
    // 获取按钮的位置和尺寸
    sf::FloatRect getBounds() const;
    void setColor(){
        sprite.setColor(sf::Color(255, 255, 255, 128));
    }
    void setPos(float x, float y);
private:
    sf::Sprite sprite;          // 用于显示按钮的精灵

    float width, height;
    float x, y;
};

class ItemButton :public Button{
    private:
    // true:角色的金钱足够购买该物品
    bool canBuy;
    // 最大购买量
    int maxBuyingNum;
    // 物品价值
    int itemCost;
    // 物品名称
    string name;
    // 物品特性
    string feature;
    // 相关联的物品
    //shared_ptr<Item> item;
    // index
    int indexOfItems;
    public:
    ItemButton(float x,float y,float width,float height, const string&textureFile,bool canBuy,int maxBuyingNum,int cost,int id):
    Button(x,y,width,height,textureFile),canBuy(canBuy),maxBuyingNum(maxBuyingNum),itemCost(cost),indexOfItems(id){
    }
    bool canWeBuy(int numOfCoins){
        canBuy=(numOfCoins>=itemCost&&maxBuyingNum>0);
        if(canBuy)cutMaxBuyingNum();
        return canBuy;
        
    }
    string maxNum(){
        string s=to_string(maxBuyingNum);
        string output="当前还可购买数量: "+s;
        return output;
    }
    string cost(){
        string s=to_string(itemCost);
        string output="物品需花费硬币数: "+s;
        return output;
    }
    // 最大购买量-1
    void cutMaxBuyingNum(){
        maxBuyingNum--;
    }
    int getItemCost()const{
        return itemCost;
    }
    // 获取物品
    int getItemIndex(){
        return indexOfItems;
    }
};
class BagItemButton:public Button{
    private:
    // 属性
    wstring atb;
    // 属性对应的编号
    int index;
    // 装备状态
    bool isOn;
    // 物品名称
    wstring name;
    // 物品特性
    wstring feature;
    // 物品id
    string id;
    public:
    BagItemButton(float x,float y,float width,float height, const string&textureFile,wstring atb,int index,wstring n,wstring f,string id):
    Button(x,y,width,height,textureFile),atb(atb),index(index),isOn(false),name(n),feature(f),id(id){
    }
    // 点击对应的图标就能够切换

    int idx(){
        return index;
    }
    bool status(){
        return isOn;
    }
    wstring theName(){
        wstring s=L"装备名称: "+name+L" 装备类型: "+atb;
        return s;
    }
    // 描述物品的功能
    wstring function(){
        wstring s=feature;
        return s;
    }
    string getItemId()const{
        return id;
    }

};


#endif

