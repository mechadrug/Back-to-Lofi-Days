#include "../include/SidebarManager.h"

SidebarManager::SidebarManager() 
    : isSidebarOpen(false), shopSystem(nullptr), achievementsSystem(nullptr), backpackSystem(nullptr), activeSystem(nullptr),button(0,740,50,50,"../resources/images/openSidebar.png"),hintButton(0,600,50,50,"../resources/images/hint.png") {
    // 设置侧边栏背景
    sidebar.setSize(sf::Vector2f(50, 300));
    sidebar.setFillColor(sf::Color(0, 0, 0, 150));  // 半透明黑色背景
    sidebar.setPosition(-50, 400);  // 初始位置在屏幕外
    buttonMask.setSize(Vector2f(50.f,50.f));
    buttonMask.setFillColor(Color(0, 0, 0, 200));
    buttonMask.setPosition(0, 740);
    playerMask.setSize(Vector2f(480.f,50.f));
    playerMask.setFillColor(Color(0,0,0,200));
    playerMask.setPosition(0,0);
    button.setColor();
    hintButton.setColor();
    moneySprite.setPosition(10.f,10.f);
    moneySprite.setTexture(monesy);
    healthSprite.setPosition(90.f,10.f);
    healthSprite.setTexture(health);
    equipmentSprite.setPosition(170.f,10.f);
    equipmentSprite.setTexture(equipment);
    // 初始化各个系统
    shopSystem = new Shop();
    achievementsSystem = new Achievements();
    backpackSystem = new Bag();
    rulesSystem = new Rule();
    hintOpen=false;

}

SidebarManager::~SidebarManager() {
    delete shopSystem;
    delete achievementsSystem;
    delete backpackSystem;
    delete rulesSystem;
}

void SidebarManager::render(sf::RenderWindow& window,MovableObject &girl) {
    window.draw(buttonMask);
    button.draw(window);
    window.draw(sidebar);
    window.draw(playerMask);
    window.draw(moneySprite);
    window.draw(healthSprite);
    window.draw(equipmentSprite);
    Text info1,info2,info3;
    info1.setFont(fonte);
    info2.setFont(fonte);
    info3.setFont(fonte);

    wstring money=to_wstring(girl.getMoney());
    info1.setString(money);
    info1.setCharacterSize(20);
    info1.setFillColor(Color::White);
    info1.setPosition(45.f,15.f);
    window.draw(info1);
    wstring curHealth=to_wstring(girl.getHealth().first)+"/"+to_wstring(girl.getHealth().second);
    info2.setString(curHealth);
    info2.setCharacterSize(20);
    info2.setFillColor(Color::White);
    info2.setPosition(135.f,15.f);
    window.draw(info2);
    wstring iteminfo;
    for(int i=0;i<=2;i++){
        char c=to_string(i)[0];
        if(girl.equippedItems.find(c)!=girl.equippedItems.end()){
            int curItem=girl.equippedItems[c];
            wstring s=items[curItem].name+" ";
            iteminfo+=s;
        }
    }
    if(!girl.reusableItems.empty()){
        auto it=girl.reusableItems.begin();
        wstring s=items[(it)->second].name;
        iteminfo+=s;
    }
    info3.setString(iteminfo);
    info3.setCharacterSize(20);
    info3.setFillColor(Color::White);
    info3.setPosition(205.f,15.f);
    window.draw(info3);
    if(usingSystem==0){
        shopSystem->update(window,girl);
        shopSystem->render(window);
        achievementsSystem->update(window);
        achievementsSystem->render(window);
        backpackSystem->update(window,girl);
        backpackSystem->render(window,girl);
        rulesSystem->update(window,girl);
        rulesSystem->render(window);
        hintButton.draw(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if(hintClock.getElapsedTime().asSeconds()<clickCooldown){
                return;
            }
            if(hintOpen){
                hintOpen=!hintOpen;
                hintClock.restart();
                return;
            }  
            if(hintButton.isPressed(sf::Mouse::getPosition(window))){
                hintMask.setSize(Vector2f(600.f,50.f));
                hintMask.setFillColor(Color(0,0,0,200));
                hintMask.setPosition(0.f,740.f);
                wstring s=hints[c_idx+1];
                info.setFont(fonte);
                info.setString(s);
                info.setCharacterSize(36);
                info.setPosition(10.f,750.f);
                hintOpen=!hintOpen;
                hintClock.restart();
            }
        }
        if(hintOpen){
            window.draw(hintMask);
            window.draw(info);
        }
        
    }
    else if(usingSystem==1){
        shopSystem->update(window,girl);
        shopSystem->render(window);
    }
    else if(usingSystem==2){
        achievementsSystem->update(window);
        achievementsSystem->render(window);
    }
    else if(usingSystem==3){
        backpackSystem->update(window,girl);
        backpackSystem->render(window,girl);
    }
    else if(usingSystem==4){
        rulesSystem->update(window,girl);
        rulesSystem->render(window);
    }

}


void SidebarManager::openSidebar() {
    isSidebarOpen = true;
}

void SidebarManager::closeSidebar() {
    isSidebarOpen = false;
    activeSystem = nullptr;  // 收起时不显示任何系统
}

