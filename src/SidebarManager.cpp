#include "../include/SidebarManager.h"

SidebarManager::SidebarManager() 
    : isSidebarOpen(false), shopSystem(nullptr), achievementsSystem(nullptr), backpackSystem(nullptr), activeSystem(nullptr),button(0,740,50,50,"../resources/images/openSidebar.png") {
    // 设置侧边栏背景
    sidebar.setSize(sf::Vector2f(50, 300));
    sidebar.setFillColor(sf::Color(0, 0, 0, 150));  // 半透明黑色背景
    sidebar.setPosition(-50, 400);  // 初始位置在屏幕外
    buttonMask.setSize(Vector2f(50.f,50.f));
    buttonMask.setFillColor(Color(0, 0, 0, 200));
    buttonMask.setPosition(0, 740);
    button.setColor();

    // 初始化各个系统
    shopSystem = new Shop();
    achievementsSystem = new Achievements();
    backpackSystem = new Bag();
    rulesSystem = new Rule();
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
    if(usingSystem==0){
        shopSystem->update(window,girl);
        shopSystem->render(window);
        achievementsSystem->update(window);
        achievementsSystem->render(window);
        backpackSystem->update(window,girl);
        backpackSystem->render(window,girl);
        rulesSystem->update(window,girl);
        rulesSystem->render(window);
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
    
    
    // if (isSidebarOpen) {
    //     window.draw(sidebar);  // 渲染侧边栏背景

    //     if (activeSystem != nullptr) {
    //         // 渲染当前活跃的子系统
    //         if (activeSystem == shopSystem) {
    //             shopSystem->render(window);
    //         } else if (activeSystem == achievementsSystem) {
    //             achievementsSystem->render(window);
    //         } else if (activeSystem == backpackSystem) {
    //             backpackSystem->render(window);
    //         }
    //     }
    // }
}

void SidebarManager::handleClick(const sf::Vector2i& mousePos,MovableObject &girl) {
    if (isSidebarOpen && activeSystem != nullptr) {
        // 处理当前激活的子系统的点击事件
        if (activeSystem == shopSystem) {
            shopSystem->handleClick(mousePos,girl);
        } else if (activeSystem == achievementsSystem) {
            achievementsSystem->handleClick(mousePos);
        } else if (activeSystem == backpackSystem) {
            backpackSystem->handleClick(mousePos);
        }
    }
}

void SidebarManager::update(sf::RenderWindow& window,MovableObject &girl) {
    if (isSidebarOpen && activeSystem != nullptr) {
        // 更新当前激活的子系统
        if (activeSystem == shopSystem) {
            shopSystem->update(window,girl);
        } else if (activeSystem == achievementsSystem) {
            achievementsSystem->update(window);
        } else if (activeSystem == backpackSystem) {
            backpackSystem->update(window,girl);
        }
    }
}

void SidebarManager::openSidebar() {
    isSidebarOpen = true;
}

void SidebarManager::closeSidebar() {
    isSidebarOpen = false;
    activeSystem = nullptr;  // 收起时不显示任何系统
}

void SidebarManager::setActiveSystem(void* system) {
    // 如果之前已经有激活的系统，先关闭它
    if (activeSystem != nullptr) {
        activeSystem = nullptr;  // 可以在这里添加关闭逻辑，避免切换时同时渲染多个系统
    }
    activeSystem = system;
}
