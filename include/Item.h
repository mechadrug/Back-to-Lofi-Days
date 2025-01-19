#ifndef ITEM_H
#define ITEM_H


#include<string>
#include <codecvt>
#include <locale>
#include <unordered_map>
#include "../SFML/Window.hpp"
#include <memory>
using namespace std;
using namespace sf;

struct Item{
    // 物品名
    wstring name;
    // 物品描述
    wstring description;
    // 物品花费
    int cost;
    // 物品编号
    string id;
    // 额外增加的隐身时间 1
    float extraInvisiTime;
    // 额外增加的攻击力 2
    int extraAtk;
    // 额外的冷却时间(可正可负) 3
    float extraCoolDown;
    // 额外的血量回复 4
    int extraBlood;
    // 额外的弹跳速度 5
    float extraJumpSpeed;
    // 额外的水平速度 6
    float extraSpeed;
    // 额外的不被攻击概率 7
    float extraProb;
    // 攻击范围 8
    int extraRange;
    // 额外的血量上限 9
    int extraBloodCap;

    int quantity;  // 用来追踪该物品的数量
    Item()=default;
    Item(const wstring&n,const wstring&d,int c,const string&i,
    float ei,int ea,float ec,int eb,float ej,float es,float ep,int er,int ebc):
    name(n),description(d),cost(c),id(i),extraInvisiTime(ei),extraAtk(ea),
    extraCoolDown(ec),extraBlood(eb),extraJumpSpeed(ej),extraSpeed(es),extraProb(ep),extraRange(er),extraBloodCap(ebc){
        quantity=0;
    }
    // 使用物品时减少数量
    void use() {
        if (quantity > 0) {
            if(this->id=="31"||this->id=="41"||this->id=="42"){
                this->quantity--;
            }
        } 
    }
    int returnQ(){
        return this->quantity;
    }
};


#endif

