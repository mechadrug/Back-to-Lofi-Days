# include"../include/GlobalVar.h"
bool gamePaused = false; 
std::wstring stringToWStringhh(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

unordered_map<int,wstring> attribute={{0,L"近战/远程"},{1,L"防御"},{2,L"增益"},{3,L"回血"},{4,L"功能"}};

unordered_map<string,string> filename={
    {"01","../resources/images/Item/moonNecklace.png"},
    {"02","../resources/images/Item/BubbleGumRing.png"},
    {"03","../resources/images/Item/meoMeoSunGlasses.png"},
    {"04","../resources/images/Item/naiLoong.png"},
    {"05","../resources/images/Item/tromboneOrna.png"},
    {"11","../resources/images/Item/baseballCap.png"},
    {"12","../resources/images/Item/darkCloak.png"},
    {"21","../resources/images/Item/flossScarf.png"},
    {"22","../resources/images/Item/springSprouw.png"},
    {"23","../resources/images/Item/coconut.png"},
    {"31","../resources/images/Item/puff.png"},
    {"41","../resources/images/Item/trumpette.png"},
    {"42","../resources/images/Item/harp.png"},
    {"43","../resources/images/Item/noteDo.png"},
    {"44","../resources/images/Item/noteRe.png"},
    {"45","../resources/images/Item/noteMi.png"},
    {"46","../resources/images/Item/noteFa.png"},
    {"47","../resources/images/Item/noteSol.png"},
    {"48","../resources/images/Item/noteLa.png"},
    {"49","../resources/images/Item/noteTi.png"}
};
unordered_map<int,string> acname={
    {1,"../resources/images/AC/ACrookie.png"},
    {2,"../resources/images/AC/ACantiFreeze.png"},
    {3,"../resources/images/AC/ACempty.png"},
    {4,"../resources/images/AC/ACdizzy.png"},
    {5,"../resources/images/AC/ACnotbad.png"},
    {6,"../resources/images/AC/ACRome.png"},
    {7,"../resources/images/AC/ACwhere.png"},
    {8,"../resources/images/AC/ACroad.png"},
    {9,"../resources/images/AC/ACfix.png"},
    {10,"../resources/images/AC/ACsequence.png"},
    {11,"../resources/images/AC/ACslam.png"},
    {12,"../resources/images/AC/ACDo.png"},
    {13,"../resources/images/AC/ACRe.png"},
    {14,"../resources/images/AC/ACMi.png"},
    {15,"../resources/images/AC/ACFa.png"},
    {16,"../resources/images/AC/ACSol.png"},
    {17,"../resources/images/AC/ACLa.png"},
    {18,"../resources/images/AC/ACTi.png"}

};

unordered_map<int,wstring>acdetails={
    {1,L"打败第一只普通史莱姆"},
    {2,L"打败第一只冰史莱姆"},
    {3,L"成功使用小号果驱赶游走史莱姆"},
    {4,L"成功通关普通迷宫"},
    {5,L"准确识别生日歌中人声的曲调"},
    {6,L"成功在斗兽场赢下胜利"},
    {7,L"成功通关镜像"},
    {8,L"成功通过大决战"},
    {9,L"晕眩追踪史莱姆3次或以上"},
    {10,L"成功找回猫咪"},
    {11,L"胜利时,每样物品至少剩下1件"},
    {12,L"在商店购买音符Do"},
    {13,L".在迷宫获得音符Re"},
    {14,L"复刻歌曲获得音符Mi"},
    {15,L"获得音符Fa"},
    {16,L"走出镜像获得音符Sol"},
    {17,L"逃离史莱姆的追赶获得音符La"},
    {18,L"成功通过大决战,获得音符Ti"}
};
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
    Item(L"长号挂饰",L"提升攻击力,攻击速度和攻击范围",25,"05",0,1,-0.3f,0,0.f,0.f,0.f,3,0),
    Item(L"棒球帽",L"提升血量上限",5,"11",0.f,0,0.f,0,0.f,0.f,0.f,0,5),
    Item(L"斗篷",L"有概率被攻击不会掉血",5,"12",0.f,0,0.f,0,0.f,0.f,0.4f,0,0),
    Item(L"棉花糖围巾",L"提升移速",7,"21",0.f,0,0.f,0,0.f,100.f,0.f,0,0),
    Item(L"弹簧",L"提升弹跳能力",9,"22",0.f,0,0.f,0,100.f,0.f,0.f,0,0),
    Item(L"椰子壳",L"延长隐身时间",8,"23",2.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"泡芙",L"回血",1,"31",0.f,0,0.f,1,0.f,0.f,0.f,0,0),
    Item(L"小号果",L"使用后,驱散游走史莱姆",10,"41",0.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"竖琴",L"使用后,晕眩史莱姆(随机史莱姆除外)",10,"42",0.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"音符Do",L"第一块碎片",0,"43",0.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"音符Re",L"第二块碎片",1000,"44",0.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"音符Mi",L"第三块碎片",1000,"45",0.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"音符Fa",L"第四块碎片",1000,"46",0.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"音符Sol",L"第五块碎片",1000,"47",0.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"音符Ra",L"第六块碎片",1000,"48",0.f,0,0.f,0,0.f,0.f,0.f,0,0),
    Item(L"音符Ti",L"第六七块碎片",1000,"49",0.f,0,0.f,0,0.f,0.f,0.f,0,0),

};

int usingSystem=0;

float globalScaleX=0.f;
float globalScaleY=0.f;
//false:未达成 true:达成
unordered_map<int,bool>acCheck={
    {1,false},
    {2,false},
    {3,false},
    {4,false},
    {5,false},
    {6,false},
    {7,false},
    {8,false},
    {9,false},
    {10,false},
    {11,false},
    {12,false},
    {13,false},
    {14,false},
    {15,false},
    {16,false},
    {17,false},
    {18,false},
};

unordered_map<int, wstring> gameRules = {
    {0, L"规则"},  // Header
    {1, L"1. 游戏目标: 扮演 Lofi Girl 离开虫洞"},
    {2, L"2. 操作: A--向左移动   D--向右移动   Space--跳跃 W--上梯子 S--下梯子"},
    {3, L"R--使用道具 J--攻击(近距离&&远距离取决于装备的效果) H--隐身 P--躲避"},
    {4, L"3. 交互: 对于可拾取目标, 经过目标所在坐标即可获取"},
    {5, L"4. 注意事项: 跳到水和火里会死掉. 血条清空会死掉."},
    {6, L"5. 商店系统购买物品: 鼠标右键查看并更新物品信息; 左键点击直接购买,"},
    {7, L"消耗型道具每样可以购买多个, 但不能超过上限; 增益型物品一旦购买, 永久拥有."},
    {8, L"6. 背包系统左键装备物品: 增益型物品每种只能同时装备一个, 效果可叠加;"},
    {9, L"背包中右键查看道具效果,道具可以重复使用,用完为止. 用完会在背包中消失."},
    {10, L"7. 其他: 在游戏界面中, 点击左下角的 '呼出侧边栏' 按钮相当于暂停,"},
    {11, L"背包, 商店, 提示, 规则还有成就系统均可以在此查看."},
    {12,L"当前角色的血量和装备状态也可以通过点击 '呼出侧边栏' 按钮来显示哦!"},
    {13, L"其他细节在游戏中再慢慢探索吧!"}
};
unordered_map<int, wstring> hints = {
    {1, L"在图一中,尽可能收集资源,打败普通史莱姆以获得成就吧"},
    {2, L"在图二中,使用小号果可以轻松驱散随机游走史莱姆!"},
    {3, L"注意脚下!"},
    {4, L"高低的柱子,从左往右代表了Do,#Do,Re,#Re,Mi,Fa,#Fa,Sol,#Sol,La,#La,Ti,D^o.唱生日歌的人都唱出了哪些音符呢?"},
    {5, L"斗兽场!!请务必活下来击败所有敌人"},
    {6, L"两只眼睛可能要分开看左右了..."},
    {7, L"大乱斗!打不过也许可以跑.?或者..你可以从别的入口.."},
    {8, L"巧妙地运用竖琴吧!追踪史莱姆触碰到你会造成巨额伤害!"},
    {9, L"1+1=2,1+2=3...这是什么数列?"}

};
bool checkJumpAndMove=false;
float DELTATIME=0.1f;
AudioManager&audio=AudioManager::getInstance();

unordered_map<pair<int,int>,bool,pair_hash>rightCube={
    {{24,3},true},
    {{24,9},true},
    {{24,15},true},
    {{24,18},true},
    {{24,24},true},
    {{24,30},true},
    {{23,33},true},
    {{24,36},true},
    {{23,6},false},
    {{23,12},false},
    {{23,21},false},
    {{23,27},false},
    {{24,34},false}

};
unordered_map<pair<int,int>,bool,pair_hash>rightTree={
    {{29,23},true},
    {{29,22},true},
    {{28,21},true},
    {{27,20},true},
    {{25,18},true},
    {{22,15},true},
    {{17,10},true},
    {{9,2},true},
    {{23,6},false},
    {{26,19},false},
    {{24,17},false},
    {{23,16},false},
    {{21,14},false},
    {{20,13},false},
    {{19,12},false},
    {{18,11},false},
    {{16,9},false},
    {{15,8},false},
    {{14,7},false},
    {{13,6},false},
    {{12,5},false},
    {{11,4},false},
    {{10,3},false}

};
int c_idx=0;
Font fonte;

bool map4win=false;
bool map5win=false;
bool map9win=false;
bool getSevenNotesAndWin=false;
bool getCat=false;
unordered_map<int,bool>noteOwnStatus={
    {13,false},//do
    {14,false},//re
    {15,false},//mi
    {16,false},//fa
    {17,false},//sol
    {18,false},//la
    {19,false}//ti
};

int gameLoad=0;
