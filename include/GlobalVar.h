#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include<string>
#include <codecvt>
#include <locale>
#include <unordered_map>
#include <SFML/Window.hpp>
#include <memory>
#include "Item.h"
#include "AudioManager.h"
#include <utility>
#include <SFML/Graphics.hpp>
#include "TexturePool.h"
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


extern vector<Item> items;

/*当前正在使用的系统:系统1:Shop 系统2:Backpack 系统3:Achievements 系统4:规则*/
extern int usingSystem;

extern float globalScaleX;
extern float globalScaleY;

extern unordered_map<int,bool>acCheck;

extern unordered_map<int,wstring>gameRules;

extern unordered_map<int,wstring>hints;

extern bool checkJumpAndMove;

extern float DELTATIME;
extern AudioManager&audio;
struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};
extern unordered_map<pair<int,int>,bool,pair_hash>rightCube;
extern unordered_map<pair<int,int>,bool,pair_hash>rightTree;

extern int c_idx;

extern Font fonte;

extern bool map4win;
extern bool map5win;
extern bool map9win;
extern bool getSevenNotesAndWin;

extern bool getCat;

extern unordered_map<int,bool>noteOwnStatus;

extern int gameLoad;

#endif