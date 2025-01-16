#include "../include/Achievements.h"

Achievements::Achievements() : isAcOpen(false), acButton(-50, 450, 50, 50, "../resources/images/hint.png") {
    // 设置商店界面的背景（简单的矩形，实际上你可以加载更复杂的界面）
    fx=window_sz.x/2-300.f;
    fy=window_sz.y/2-225.f;
    acWindow.setSize(sf::Vector2f(600, 450));
    acWindow.setFillColor(sf::Color(0, 0, 0, 200));  // 半透明黑色背景
    acWindow.setPosition(fx, fy);  // 商店界面的初始位置
    buttonMask.setSize(Vector2f(50,50));
    buttonMask.setFillColor(Color(0, 0, 0, 200));
    buttonMask.setPosition(-50, 450);
    acButton.setColor();
    loadFont("../resources/fonts/Silver.ttf");
    loadAC();
}
void Achievements::loadAC(){
    for(int i=1;i<=18;i++){
        int dx=i<=9?0:1;
        acButtons.push_back(ACButton(fx+dx*300.f,fy+50.f*((i-1)%9),50,50,acname[i],acdetails[i],i));
        showButtons.push_back(ACButton(fx+dx*300.f+55.f,fy+50.f*((i-1)%9),250,50,"../resources/images/AC/ACview.png",acdetails[i],i));
    }
}
void Achievements::update(sf::RenderWindow& window) {
    // 检查鼠标点击并处理按钮交互
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        handleClick(sf::Mouse::getPosition(window));
    }

    // 如果商店打开，可以在这里添加更多的商店交互逻辑
}

void Achievements::render(sf::RenderWindow& window) {
    acButton.draw(window);  // 渲染商店按钮

    // 如果商店界面打开，绘制商店界面
    if (isAcOpen) {
        window.draw(acWindow);
        for(auto&showac:showButtons){
            showac.draw(window);
        }
        Text infortext;
        infortext.setFont(font);
        for(auto&ac:acButtons){
            ac.draw(window);
            int id=ac.getId();
            int dx=id<=9?0:1;
            // 如果成就未达成并且成就显示为true,才展示具体的成就达成方式
            if(acCheck[id]==false&&ac.getIfShowdt()){
                infortext.setString(ac.getDetails());
                
            }else if(acCheck[id]==true){
                infortext.setString(L"成就已完成!");
            }else{
                infortext.setString(L"成就未完成..");
            }
            infortext.setCharacterSize(18);
            infortext.setFillColor(Color::Black);
            infortext.setPosition(fx+dx*300.f+65.f,16.f+fy+50.f*((id-1)%9));
            window.draw(infortext);
        }
    }
}

void Achievements::handleClick(const sf::Vector2i& mousePos) {
    if (clickClock.getElapsedTime().asSeconds() < clickCooldown) {
        return;  // 如果没过 1 秒，则不执行点击操作
    }
    if(isAcOpen){
        showACInfo(mousePos);
    }
    // 如果鼠标点击的位置在商店按钮范围内
    if (acButton.isPressed(mousePos)) {
        audio.playSoundEffect(SoundChannel::System,"select",SoundPriority::MEDIUM);

        if (isAcOpen) {
            closeShop();  // 关闭商店
            //gamePaused = false;  // 恢复游戏
            usingSystem=0;
        } else {
            openShop();  // 打开商店
            //gamePaused = true;  // 暂停游戏
            usingSystem=2;
        }
        clickClock.restart();
    }
}
void Achievements::showACInfo(const sf::Vector2i& mousePos) {
    // 遍历成就按钮，找到鼠标左键点击的成就按钮，显示需要完成该成就要达成的目标
    for (auto& AC : acButtons) {
        if (AC.isPressed(mousePos)) {
            audio.playSoundEffect(SoundChannel::System,"select",SoundPriority::MEDIUM);

            AC.changeShowdt();
            clickClock.restart();
            return;
        }
    }
}
void Achievements::openShop() {
    isAcOpen = true;
}

void Achievements::closeShop() {
    isAcOpen = false;
}

void Achievements::setPos(float x, float y){
    buttonMask.setPosition(x,y);
    acButton.setPos(x,y);
}
