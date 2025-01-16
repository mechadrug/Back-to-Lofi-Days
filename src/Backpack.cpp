#include "../include/Backpack.h"

Bag::Bag() : isBagOpen(false), bagButton(-50, 500, 50, 50, "../resources/images/bag.png") {
    // 设置商店界面的背景（简单的矩形，实际上你可以加载更复杂的界面）
    fx=window_sz.x/2-300.f;
    fy=window_sz.y/2-225.f;
    bagWindow.setSize(sf::Vector2f(600,450));
    bagWindow.setFillColor(sf::Color(0, 0, 0, 200));  // 半透明黑色背景
    bagWindow.setPosition(fx,fy);  // 背包界面的初始位置
    buttonMask.setSize(Vector2f(50,50));
    buttonMask.setFillColor(Color(0, 0, 0, 200));
    buttonMask.setPosition(-50, 500);
    bagButton.setColor();
    loadFont("../resources/fonts/Silver.ttf");
    //loadItems();
}
// 一开始应该是没有loadItems的,应该需要girls去处理这个事件
void Bag::loadItems(MovableObject&girl){
    // 测试编号从0~5
    // Texture&texture0=TexturePool::getTexture("../resources/images/bag.png");
    // Texture&texture1=TexturePool::getTexture("../resources/images/bag.png");
    // Texture&texture2=TexturePool::getTexture("../resources/images/bag.png");
    // Texture&texture3=TexturePool::getTexture("../resources/images/bag.png");
    // Texture&texture4=TexturePool::getTexture("../resources/images/bag.png");
    // Texture&texture5=TexturePool::getTexture("../resources/images/bag.png");
    vector<BagItemButton>temp;
    int row=0,col=0;
    for (auto& item : girl.getBag()) {
        //int id=item.second.id[0]-'0';
        string id=items[item.second].id;
        row=id[0]-'0';
        col=id[1]-'0';
        float xPos = fx + (col-1) * 60.f;
        float yPos = fy + row * 60.f;
        // if(id=="31"){
        // cout<<item.second.quantity<<endl;
        // }
        if((id=="31"||id=="41"||id=="42")&&items[item.second].quantity==0){
            // cout<<"te"<<endl;
            continue;
        }
        temp.push_back(BagItemButton(xPos, yPos, 50, 50, filename[id], attribute[row], row,items[item.second].name, items[item.second].description,id));
    }
    itemButtons.clear();
    itemButtons=move(temp);
    // itemButtons.push_back(BagItemButton(fx,fy,50,50,"../resources/images/bag.png",L"近战",0,L"雷电",L"提升伤害和攻速"));

}

void Bag::update(sf::RenderWindow& window,MovableObject&girl) {
    // 检查鼠标点击并处理按钮交互
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        handleClick(sf::Mouse::getPosition(window));
        handleItemSelection(window, girl);
    }
    if(Mouse::isButtonPressed(Mouse::Right)){
        showItemInfo(Mouse::getPosition(window),window);
    }
    
    // 如果背包打开，可以在这里添加更多的背包交互逻辑
}

void Bag::render(sf::RenderWindow& window,MovableObject&girl) {
    bagButton.draw(window);  // 渲染商店按钮

    // 如果商店界面打开，绘制商店界面
    if (isBagOpen) {
        window.draw(bagWindow);
        loadItems(girl);
        for(auto& item:itemButtons){
            item.draw(window);
        }
        if(!currentItemInfo.empty()){
            Text infortext;
            infortext.setFont(font);
            infortext.setString(currentItemInfo);
            infortext.setCharacterSize(24);
            infortext.setFillColor(Color::White);
            infortext.setPosition(fx,fy+370.f);
            window.draw(infortext);
        }
    }
}

void Bag::handleClick(const sf::Vector2i& mousePos) {
    if (clickClock.getElapsedTime().asSeconds() < clickCooldown) {
        return;  // 如果没过 1 秒，则不执行点击操作
    }
    
    // 如果鼠标点击的位置在背包按钮范围内
    if (bagButton.isPressed(mousePos)) {
        audio.playSoundEffect(SoundChannel::System,"select",SoundPriority::MEDIUM);
        if (isBagOpen) {
            closeBag();  // 关闭背包
            //gamePaused = false;  // 恢复游戏
            usingSystem=0;
        } else {
            openBag();  // 打开背包
            //gamePaused = true;  // 暂停游戏
            usingSystem=3;
        }
        clickClock.restart();
    }
}

void Bag::showItemInfo(const sf::Vector2i& mousePos, sf::RenderWindow& window) {
    // 遍历物品按钮，找到鼠标右键点击的物品，显示物品详细信息
    for (auto& itemButton : itemButtons) {
        if (itemButton.isPressed(mousePos)) {
        audio.playSoundEffect(SoundChannel::System,"select",SoundPriority::MEDIUM);
            currentItemInfo = itemButton.theName()+"\n"+itemButton.function();  // 获取物品的详细信息
            return;
        }
    }
    // 如果右键点击没有在物品上，清空详细信息
    currentItemInfo.clear();
}

void Bag::openBag() {
    isBagOpen = true;
}

void Bag::closeBag() {
    isBagOpen = false;
}

void Bag::setPos(float x, float y){
    buttonMask.setPosition(x,y);
    bagButton.setPos(x,y);
}
void Bag::equipItem(int ID,MovableObject&girl){
    char itemType=items[ID].id[0];
    // 移除原先物品的效果
    if (girl.equippedItems.find(itemType) != girl.equippedItems.end()) {
        // 如果已有装备，卸下当前装备的物品
        int currentEquippedItem = girl.equippedItems[itemType];
        removeItemEffect(currentEquippedItem, girl);  // 移除当前物品的增益效果
    }
    if (items[ID].id == "31" || items[ID].id == "41" || items[ID].id == "42") {  // 可重复使用物品
        girl.reusableItems.clear();
        girl.reusableItems[items[ID].id] =ID;  // 存储物品而不是直接应用效果
    }else{
            //应用新物品的效果
        if (items[ID].id == "01") {  // 月牙项链 (提升攻击力)
            girl.changeAttack(items[ID].extraAtk);
        } else if (items[ID].id == "02") {  // 泡泡糖戒指 (提升攻击速度)
            girl.changeAttackCooldown(items[ID].extraCoolDown);  // 假设增加攻击速度通过改变攻击冷却时间来实现
        } else if (items[ID].id == "03") {  // 鱼骨墨镜 (提升攻击力和攻速)
            girl.changeAttack(items[ID].extraAtk);
            girl.changeAttackCooldown(items[ID].extraCoolDown);
        } else if (items[ID].id == "04") {  // 奶龙玩偶 (提升攻击力和攻击范围)
            girl.changeAttack(items[ID].extraAtk);
            girl.changeAtkRange(items[ID].extraRange);
        } else if (items[ID].id == "05") {  // 长号挂饰 (提升速度和攻击范围)
            girl.changeSpeed(items[ID].extraSpeed);
            girl.changeAtkRange(items[ID].extraRange);
        } else if (items[ID].id == "11") {  // 棒球帽 (提升血量上限)
            girl.changeHealthCap(items[ID].extraBloodCap);
        } else if (items[ID].id == "12") {  // 斗篷 (有概率被攻击不会掉血)
            girl.changeInviProb(items[ID].extraProb);
        } else if (items[ID].id == "21") {  // 棉花糖围巾 (提升移速)
            girl.changeSpeed(items[ID].extraSpeed);
        } else if (items[ID].id == "22") {  // 弹簧 (提升弹跳能力)
            girl.changeJumpHeight(items[ID].extraJumpSpeed);
        } else if (items[ID].id== "23") {  // 椰子壳 (延长隐身时间)
            girl.changeInvisiTime(items[ID].extraInvisiTime);
        }
        girl.equippedItems[itemType] = ID;
        // 使用物品（减少数量）
        items[ID].use();
    }
    
    
}
void Bag::handleItemSelection(sf::RenderWindow& window, MovableObject& girl) {
    // 如果按下左键，装备当前选中的物品
    if (clickClock.getElapsedTime().asSeconds() < clickCooldown) {
        return;  // 如果没过 1 秒，则不执行点击操作
    }
        for (auto& itemButton : itemButtons) {
            if (itemButton.isPressed(sf::Mouse::getPosition(window))) {
                audio.playSoundEffect(SoundChannel::System,"equipItem",SoundPriority::MEDIUM);

                // 获取当前物品

                string itemId = itemButton.getItemId();
                int selectedItem = girl.getBag()[itemId];

                // 装备物品
                equipItem(selectedItem,girl);
                std::cout << "Equipped item: " << items[selectedItem].id << std::endl;
                clickClock.restart();
            }
        }
    
}