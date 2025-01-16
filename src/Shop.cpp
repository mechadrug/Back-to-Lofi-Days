#include "../include/Shop.h"

Shop::Shop() : isShopOpen(false), shopButton(-50, 400, 50, 50, "../resources/images/shop.png") {
    // 设置商店界面的背景（简单的矩形，实际上你可以加载更复杂的界面）
    fx=window_sz.x/2-300.f;
    fy=window_sz.y/2-225.f;
    shopWindow.setSize(sf::Vector2f(600, 450));
    shopWindow.setFillColor(sf::Color(0, 0, 0, 200));  // 半透明黑色背景
    shopWindow.setPosition(fx, fy);  // 商店界面的初始位置
    buttonMask.setSize(Vector2f(50,50));
    buttonMask.setFillColor(Color(0, 0, 0, 200));
    buttonMask.setPosition(-50, 400); // init pos out of window
    shopButton.setColor();
    loadFont("../resources/fonts/Silver.ttf");
    loadItems();
}

void Shop::loadItems(){
    // Texture&texture1=TexturePool::getTexture("../resources/images/shop.png");
    // Texture&texture2=TexturePool::getTexture("../resources/images/shop.png");
    // line1
    itemButtons.push_back(ItemButton(fx,fy,50,50,"../resources/images/Item/moonNecklace.png",false,1,5,0));
    itemButtons.push_back(ItemButton(fx+60,fy,50,50,"../resources/images/Item/BubbleGumRing.png",false,1,3,1));
    itemButtons.push_back(ItemButton(fx+120,fy,50,50,"../resources/images/Item/meoMeoSunGlasses.png",false,1,3,2));
    itemButtons.push_back(ItemButton(fx+180,fy,50,50,"../resources/images/Item/naiLoong.png",false,1,3,3));
    itemButtons.push_back(ItemButton(fx+240,fy,50,50,"../resources/images/Item/tromboneOrna.png",false,1,3,4));
    // line2
    itemButtons.push_back(ItemButton(fx,fy+60,50,50,"../resources/images/Item/baseballCap.png",false,1,3,5));
    itemButtons.push_back(ItemButton(fx+60,fy+60,50,50,"../resources/images/Item/darkCloak.png",false,1,3,6));
    // line3
    itemButtons.push_back(ItemButton(fx,fy+120,50,50,"../resources/images/Item/flossScarf.png",false,1,3,7));
    itemButtons.push_back(ItemButton(fx+60,fy+120,50,50,"../resources/images/Item/springSprouw.png",false,1,3,8));
    itemButtons.push_back(ItemButton(fx+120,fy+120,50,50,"../resources/images/Item/coconut.png",false,1,3,9));
    // line4
    itemButtons.push_back(ItemButton(fx,fy+180,50,50,"../resources/images/Item/puff.png",false,5,3,10));
    // line5
    itemButtons.push_back(ItemButton(fx,fy+240,50,50,"../resources/images/Item/trumpette.png",false,5,3,11));
    itemButtons.push_back(ItemButton(fx+60,fy+240,50,50,"../resources/images/Item/harp.png",false,5,3,12));
    itemButtons.push_back(ItemButton(fx+120,fy+240,50,50,"../resources/images/Item/noteDo.png",false,1,3,13));

}

void Shop::update(sf::RenderWindow& window,MovableObject&girl) {
    // 检查鼠标点击并处理按钮交互
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        handleClick(sf::Mouse::getPosition(window),girl);
    }
    if(Mouse::isButtonPressed(Mouse::Right)){
        showItemInfo(Mouse::getPosition(window),window);
    }
    
}

void Shop::render(sf::RenderWindow& window) {
    window.draw(buttonMask);
    shopButton.draw(window);  // 渲染商店按钮

    // 如果商店界面打开，绘制商店界面
    if (isShopOpen) {
        window.draw(shopWindow);
        for(auto& item:itemButtons){
            item.draw(window);
        }
        if(!currentItemInfo.empty()){
            Text infortext;
            infortext.setFont(font);
            infortext.setString(stringToWStringhh(currentItemInfo));
            infortext.setCharacterSize(24);
            infortext.setFillColor(Color::White);
            infortext.setPosition(fx,fy+370.f);
            window.draw(infortext);
        }
    }
}

void Shop::handleClick(const sf::Vector2i& mousePos,MovableObject& girl) {
    if (clickClock.getElapsedTime().asSeconds() < clickCooldown) {
        return;  // 如果没过 1 秒，则不执行点击操作
    }
    // 购买检测
    if(isShopOpen){
        /*思路: 如果购买成功,则将对应的item装入bag内部,并提醒Backpack类去更新渲染;*/
    for (auto& itemButton : itemButtons) {
        if (itemButton.isPressed(mousePos)) {
            
            int money=girl.getMoney();
            if(itemButton.canWeBuy(money)){
                audio.playSoundEffect(SoundChannel::System,"successToBuy",SoundPriority::MEDIUM);
                girl.decreaseMoney(itemButton.getItemCost());
                girl.addItemToBag(itemButton.getItemIndex());
            }else{
                audio.playSoundEffect(SoundChannel::System,"failToBuy",SoundPriority::MEDIUM);
            }
            clickClock.restart();
        }
    }
    }
    
    // 如果鼠标点击的位置在商店按钮范围内
    if (shopButton.isPressed(mousePos)) {
        audio.playSoundEffect(SoundChannel::System,"select",SoundPriority::MEDIUM);

        if (isShopOpen) {
            closeShop();  // 关闭商店
            usingSystem=0;
        } else {
            openShop();  // 打开商店
            usingSystem=1;
        }
        clickClock.restart();
    }
    
}
void Shop::showItemInfo(const sf::Vector2i& mousePos, sf::RenderWindow& window) {
    // 遍历物品按钮，找到鼠标右键点击的物品，显示物品详细信息
    for (auto& itemButton : itemButtons) {
        if (itemButton.isPressed(mousePos)) {
            audio.playSoundEffect(SoundChannel::System,"select",SoundPriority::MEDIUM);

            currentItemInfo = itemButton.maxNum()+" "+itemButton.cost();  // 获取物品的详细信息
            return;
        }
    }
    // 如果右键点击没有在物品上，清空详细信息
    currentItemInfo.clear();
}
void Shop::openShop() {
    isShopOpen = true;
}

void Shop::closeShop() {
    isShopOpen = false;
}

void Shop::setPos(float x,float y){
    buttonMask.setPosition(x,y);
    shopButton.setPos(x,y);
}
