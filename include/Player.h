#ifndef PLAYER_H
#define PLAYER_H

#include "json.hpp"
#include <fstream>
#include <iostream>
//#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "GlobalVar.h"
#include<unordered_map>
#include<cstdlib>
#include<ctime>
// #include "Backpack.h"
// #include "Shop.h"
//#include "Item.h"
using namespace std;
using namespace sf;
using json=nlohmann::json;

class Slime;

struct Tile{
    //瓷砖的编码
    int tileType;
    //1 2 4 5
    bool isCollidable;
    //2
    bool isIce;
    //4
    bool isBrokenable;
    //3
    bool isWater;
    //5
    bool isMucous;
    //8
    bool isWindUD;
    //7
    bool isWindLR;
    //6
    bool isLadder;
    //51
    bool m1t2;
    //52
    bool m2t1;
    //53
    bool m2t3;
    //54
    bool m3t2;
    //55
    bool m3t4_1;
    //56
    bool m3t4_2;
    //57
    bool m4t3_1;
    //58
    bool m4t3_2;
    //59
    bool m3t5;
    //60
    bool m5t3;
    //61
    bool m5t6;
    //62
    bool m5t8;
    //63
    bool m6t5;
    //64
    bool m6t7;
    //65
    bool m7t6;
    //66
    bool m7t9;
    //67
    bool m7t8;
    //68
    bool m8t5;
    //69
    bool m8t7;
    //70
    bool m9t7;
    //71
    bool intoExit;
    bool isChange;
    chrono::duration<float> standingTime; //累计站立时间

};
class MovableObject{
    private:
    Sprite sprite;
    Vector2f position;
    // 6
    float speed;//横移速度6
    float verticalSpeed;//垂直速度
    float gravity;//重力加速度
    // 5
    float jumpHeight;//跳跃高度5
    bool isJumping;//是否在跳跃当中
    float inertiaSpeed;//在冰面上的惯性速度
    // 4
    int Health;//:血量4
    // 2
    int Attack;//:攻击力2
    bool intoWind;//进入风场
    // 9
    int HealthCap;// 血量上限9
    // 8
    int atkRange;// 攻击范围8
    // 7
    float probWithoutBeingAttacked; // 可能的不被攻击的概率7
    //等待实现的功能:近距离攻击;隐身;躲避
    //键位安排:
    //A: LEFT (已经实现了)
    //D: RIGHT (已经实现了)
    //SPACE: JUMP (已经实现了)
    //J: 近距离攻击 (未实现,靠近敌人一格的时候点击J可以攻击,每秒一次,造成伤害)
    //H: HIDE 隐身 (未实现,每十秒钟可以使用一次,按下之后敌人无法攻击,持续两秒(看不到玩家))
    //P: DODGING 躲避 (往前瞬移两格,有适当的无敌帧时间)
    bool isInvisible; //是否可视
    // 1
    float invisibilityDuration; //隐身的持续时间1
    float invisiTime;// 用来动态计算隐身的持续时间
    chrono::steady_clock::time_point lastInvisibilityTime;  //上次隐身使用时间
    float invisibilityCooldown;  //隐身的冷却时间
    bool isDodging;
    bool isDead;
    // 3
    float attackCooldown; // 3
    chrono::steady_clock::time_point lastAttackTime;
    std::chrono::steady_clock::time_point lastUseTime;
    // money :角色持有货币;初始值为0;
    int money;
    // 背包
    unordered_map<string,int> bag;

    // 原始值存储->对应于装备管理
    struct OriginalValues { 
        // 原始的隐身时间 1
        float invisiTime;
        // init attack 2
        int attack;
        // 原始的冷却时间 3
        float attackCooldown;
        // init health 4
        int health;
        // 原始起跳速度 5
        float jumpHeight;
        // 原始速度 6
        float speed;
        // 原始的被攻击不掉血概率 7
        float initprob;
        // 原始的攻击范围 8
        int atkRange;
        // 原始的血量上限 9
        int healthCap;
    };

    OriginalValues originalValues;

    public:
    //等待实现方法:检测冰墙(增加滑动);检测碎墙(站上去之后过一秒这个墙就不能站人了);检测水域(主角掉进水里,直接死亡,跳到结算界面)
    MovableObject()=default;
    MovableObject(float x, float y, const Texture& texture,float sx,float sy);
    void update(float deltaTime, vector<vector<Tile>>& mapData, float tileWidth, float tileHeight,vector<Slime>&slimes);
    void render(RenderWindow& window);
    void updateStandingTime(float newX,float newY, vector<std::vector<Tile>>& mapData,float tileWidth, float tileHeight);
    bool checkCollision(float newX,float newY, vector<vector<Tile>>&mapData, float tileWidth, float tileHeight,int select);//检测角色碰撞
    bool toExit( vector<vector<Tile>>& mapData, float tileWidth, float tileHeight);
    bool checkwater( vector<vector<Tile>>& mapData, float tileWidth, float tileHeight);
    bool checkIce(float newX,float newY, vector<vector<Tile>>&mapData, float tileWidth, float tileHeight);
    int changeMap(vector<vector<Tile>>& mapData, float tileWidth, float tileHeight);
    void changePositionBetweenMap(float newX,float newY){
        position.x=newX;
        position.y=newY;
        sprite.setPosition(position);
    };
    Vector2f getPosition() const {
        return position; // 返回存储的实际位置
    }
    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();  // 获取 sprite 的边界框
    }
    void takeDamage(int slimeDamage){
        float randProb=static_cast<float>(rand())/RAND_MAX;
        if(randProb>probWithoutBeingAttacked){
            Health-=slimeDamage;
        }
        if(Health<=0)isDead=true;
    }
    bool isAlive(){
        return !isDead;
    }
    void attack(vector<Slime>&slimes);

    void startInvisibility();
    void checkInvisibilityCooldowm(float deltaTime);
    bool canUseInvisibility();
    bool InvisibleForSlime(){
        return isInvisible;
    }
        // 修改数值
    void modifyValue(float& member, float valueChange) {
        member += valueChange;
    }

    void modifyValue(int& member, int valueChange) {
        member += valueChange;
    }

    // 恢复数值到初始值
    void restoreValue(float& member, float originalValue) {
        member = originalValue;
    }

    void restoreValue(int& member, int originalValue) {
        member = originalValue;
    }

    // 改变隐身时间并恢复 (1)
    void changeInvisiTime(float valueChange) {
        modifyValue(invisibilityDuration, valueChange);
    }

    void restoreInvisiTime() {
        restoreValue(invisibilityDuration, originalValues.invisiTime);
    }

    // 改变攻击力并恢复 (2)
    void changeAttack(int valueChange) {
        modifyValue(Attack, valueChange);
    }

    void restoreAttack() {
        restoreValue(Attack, originalValues.attack);
    }

    // 改变冷却时间并恢复 (3)
    void changeAttackCooldown(float valueChange) {
        modifyValue(attackCooldown, valueChange);
    }

    void restoreAttackCooldown() {
        restoreValue(attackCooldown, originalValues.attackCooldown);
    }

    // 使用泡芙回复血量 (4)
    void changeHealth(int valueChange) {
        modifyValue(Health, valueChange);
    }

    // void restoreHealth() {
    //     restoreValue(Health, originalValues.health);
    // }

    // 改变起跳速度并恢复 (5)
    void changeJumpHeight(float valueChange) {
        modifyValue(jumpHeight, valueChange);
    }

    void restoreJumpHeight() {
        restoreValue(jumpHeight, originalValues.jumpHeight);
    }

    // 改变速度并恢复 (6)
    void changeSpeed(float valueChange) {
        modifyValue(speed, valueChange);
    }

    void restoreSpeed() {
        restoreValue(speed, originalValues.speed);
    }

    // 改变被攻击不掉血概率并恢复 (7)
    void changeInviProb(float valueChange) {
        modifyValue(probWithoutBeingAttacked, valueChange);
    }

    void restoreInviProb() {
        restoreValue(probWithoutBeingAttacked, originalValues.initprob);
    }

    // 改变攻击范围并恢复 (8)
    void changeAtkRange(int valueChange) {
        modifyValue(atkRange, valueChange);
    }

    void restoreAtkRange() {
        restoreValue(atkRange, originalValues.atkRange);
    }

    // 改变血量上限并恢复 (9)
    void changeHealthCap(int valueChange) {
        modifyValue(HealthCap, valueChange);
    }

    void restoreHealthCap() {
        restoreValue(HealthCap, originalValues.healthCap);
        if(Health>=originalValues.healthCap){
            restoreValue(Health, originalValues.health);
        }
    }

    unordered_map<char,int> equippedItems; //存储每种类型的已装备物品 (根据首个字符区分)
    unordered_map<string,int> reusableItems;  // 存储可重复使用的物品
    
    // 获取money
    int getMoney() const{
        return money;
    }
    // 扣除money
    void decreaseMoney(int amount) {
        if (money >= amount) {
            money -= amount;
        }
    }
    void earnMoney(){
        money++;
    }
    bool canPurchaseMultiple(const string& itemId) {
    return itemId == "31" || itemId == "41" || itemId == "42";  // 根据物品ID判断是否可以多次购买
    }

    // 添加物品到bag里面
    void addItemToBag( int ID) {
    if (canPurchaseMultiple(items[ID].id)) {
        // 对于可以多次购买的物品，增加数量
        if (bag.find(items[ID].id) != bag.end()) {
            // 物品已存在，增加数量
            items[ID].quantity += 1;
            std::cout << "Item " << items[ID].id << " quantity increased. New quantity: " << items[ID].quantity << std::endl;
        } else {
            // 物品不存在，直接添加
            bag[items[ID].id] = ID;
            items[ID].quantity = 1;
            std::cout << "Item " << items[ID].id << " added to bag." << std::endl;
        }
    } else {
        // 对于不能多次购买的物品，只能购买一个副本
        if (bag.find(items[ID].id) == bag.end()) {
            // 物品不存在，添加到背包
            bag[items[ID].id] = ID;
            std::cout << "Item " << items[ID].id << " added to bag." << std::endl;
        } else {
            // 物品已经存在，不能重复添加
            std::cout << "Item " << items[ID].id << " is already in the bag!" << std::endl;
        }
    }
}
    unordered_map<string,int>& getBag(){
        return bag;
    }
    void triggerReusableItems();
    void applyItemEffect(int ID);
    // 检测装备函数
};
//void attack();
    //void useInvisibility();
    //void dodge();

    // int getHealth() const { return Health; }
    // void setHealth(int health) { Health = health; }

    // int getAttack() const { return Attack; }
    // void setAttack(int attack) { Attack = attack; }

    // bool getIsInvisible() const { return isInvisible; }
    // void setIsInvisible(bool invisible) { isInvisible = invisible; }

    // bool getIsDodging() const { return isDodging; }
    // void setIsDodging(bool dodging) { isDodging = dodging; }
// 敌人类：静止不动的敌人，检测玩家并自动攻击
// class Enemy {
// private:
//     int Health;           // 敌人的血量
//     int Attack;           // 敌人的攻击力
//     float attackFrequency; // 攻击频率（每秒攻击多少次）
//     Vector2f position;    // 敌人的位置
//     bool isDead;          // 敌人是否已死亡

// public:
//     Enemy(int health, int attack, float frequency, Vector2f pos);
//     void update(float deltaTime, const MovableObject& player); // 更新敌人状态
//     void attack(MovableObject& player); // 攻击玩家
//     bool isPlayerInRange(const MovableObject& player); // 检查玩家是否在攻击范围内
//     void die(); // 敌人死亡

//     // Getter 和 Setter
//     int getHealth() const { return Health; }
//     void setHealth(int health) { Health = health; }

//     int getAttack() const { return Attack; }
//     void setAttack(int attack) { Attack = attack; }

//     bool getIsDead() const { return isDead; }
// };
json load_map(const string& filename);
vector<vector<Tile>> load_map_data(const json&map_data,int width,int height);
//待实现:静止不动的敌人类,检测范围内玩家,自动发起攻击,攻击频率和玩家一致,血量比玩家少,死亡之后不再攻击玩家.如果玩家被敌人打死,就会跳到结算界面


#endif