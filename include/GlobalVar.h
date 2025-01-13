#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include<string>
#include <codecvt>
#include <locale>
#include <unordered_map>
#include <SFML/Window.hpp>
#include <memory>
#include "Item.h"
using namespace std;
using namespace sf;
extern bool gamePaused;

extern wstring stringToWStringhh(const std::string& str);

extern unordered_map<int,wstring> attribute;

extern unordered_map<string,string> filename;

extern unordered_map<int,string>acname;

extern unordered_map<int,wstring>acdetails;

extern Vector2u window_sz;

extern float fx;
extern float fy;

// 设计一种全局变量,能够在将物品添加进背包的时候,同时更改这个全局变量,接着使得Backpack根据这个全局变量进行渲染的修改.
// 给每一个item赋予一个编号,编号是有限个的
// 这样就不用真的往girl持有的背包里添加真的item
// 可以直接设计成一个全局的结构体

//extern Item items[];
extern vector<Item> items;

/*当前正在使用的系统:系统1:Shop 系统2:Backpack 系统3:Achievements 系统4:规则*/
extern int usingSystem;

extern float globalScaleX;
extern float globalScaleY;

extern unordered_map<int,bool>acCheck;

extern unordered_map<int,wstring>gameRules;
#endif