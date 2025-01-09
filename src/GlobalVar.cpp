# include"../include/GlobalVar.h"
bool gamePaused = false; 
std::wstring stringToWStringhh(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

unordered_map<int,wstring> attribute={{0,L"近战/远程"},{1,L"防御"},{2,L"增益"},{3,L"回血"},{4,L"功能"}};

// 初始的屏幕大小
Vector2u window_sz={0,0};

// 针对系统的x坐标
float fx=0;
// 针对系统的y坐标
float fy=0;
// 0~13
vector<Item> items={
    Item(L"月牙项链",L"提升攻击力",5,"01",0.f,2,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"泡泡糖戒指",L"提升攻击速度",4,"02",0.f,0,-0.4f,0,0.f,0.f,0.f,0,0),
    Item(L"鱼骨墨镜",L"提升攻击力和攻速",17,"03",0.f,3,-0.3f,0,0.f,0.f,0.f,0,0),
    Item(L"奶龙玩偶",L"提升攻击力和攻击范围(我才是奶龙)",20,"04",0,2,0.f,0,0.f,0.f,0.f,2,0),
    Item(L"长号挂饰",L"提升速度和攻击范围",25,"05",0,3,0.f,0,0.f,0.f,0.f,3,0),
    Item(L"棒球帽",L"提升血量上限",5,"11",0.f,0,0.f,0,0.f,0.f,0.f,0,5),
    Item(L"斗篷",L"有概率被攻击不会掉血",5,"12",0.f,0,0.f,0,0.f,0.f,0.4f,0,0),
    Item(L"棉花糖围巾",L"提升移速",7,"21",0.f,0,0.f,0,0.f,400.f,0.f,0,0),
    Item(L"弹簧",L"提升弹跳能力",9,"22",0.f,0,0.f,0,200.f,0.f,0.f,0,0),
    Item(L"椰子壳",L"延长隐身时间",8,"23",2.f,0,0.f,0,0.f,400.f,0.f,0,0),
    Item(L"泡芙",L"回血",1,"31",0.f,0,0.f,1,0.f,0.f,0.f,0,0),
    Item(L"小号果",L"?",10,"41",0.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"竖琴",L"?",10,"42",0.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"音符Do",L"第一块碎片",0,"43",0.f,0,0.f,0,0.f,0.f,0.f,0,0),

};

int usingSystem=0;