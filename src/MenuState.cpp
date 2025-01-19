#include "../include/MenuState.h"
#include "../include/Game.h"
#include "../include/LevelState.h"
#include "../include/TexturePool.h"
#include <iostream>
using namespace std;
 MenuState::MenuState(Game* game) : game(game) {

    //获取窗口的大小
    sf::Vector2u windowSize = game->getWindow().getSize();
    window_sz=windowSize;
    //开始按钮的宽度和高度:
     unsigned int buttonWidth = 240;
     unsigned int buttonHeight = 80;
    //结束按钮的宽度:
    unsigned int exitButttonWidth = 120;
    unsigned int bgwidth=240;
    unsigned int bgheight=80;

    unsigned int startButtonX = (windowSize.x - buttonWidth) / 2;
    unsigned int startButtonY = windowSize.y - buttonHeight - 140;

    unsigned int exitButtonX = (windowSize.x - exitButttonWidth) / 2-110;
    unsigned int exitButtonY = startButtonY + buttonHeight + 20;
    startButton=Button(startButtonX,startButtonY,buttonWidth,buttonHeight,"../resources/images/start.png");
    exitButton=Button(exitButtonX,exitButtonY,exitButttonWidth,buttonHeight,"../resources/images/exit.png");
    oldgameButton=Button(exitButtonX+130,exitButtonY,182,80,"../resources/images/oldGame.png");
    background=Map("../resources/images/menuBg.png");

    //
    // 批量加载音乐文件从game1.mp3到game10.mp3
    std::vector<std::pair<std::string, std::string>> musicFiles;
    for(int i = 1; i <= 12; ++i){
        std::string name = "game" + std::to_string(i);
        std::string path = "../resources/sounds/music/game" + std::to_string(i) + ".mp3";
        musicFiles.emplace_back(std::make_pair(name, path));
    }

    // 批量加载音乐文件
    audio.loadMusicFiles(musicFiles);
    audio.loadMusic("birthday","../resources/sounds/music/zmusicHappyBirthday.mp3");
    audio.loadMusic("tail","../resources/sounds/music/zmusicTailSong.mp3");
    audio.loadSoundEffect("attack","../resources/sounds/attack.wav");
    audio.loadSoundEffect("beHitted","../resources/sounds/beHitted.wav");
    audio.loadSoundEffect("bloodUp","../resources/sounds/bloodUp.wav");
    audio.loadSoundEffect("bobSlimeATK","../resources/sounds/bobSlimeATK.wav");
    audio.loadSoundEffect("bobSlimeDie","../resources/sounds/bobSlimeDie.wav");
    audio.loadSoundEffect("drop","../resources/sounds/drop.wav");
    audio.loadSoundEffect("equipItem","../resources/sounds/equipItem.wav");
    audio.loadSoundEffect("failToBuy","../resources/sounds/failToBuy.wav");
    audio.loadSoundEffect("fetch","../resources/sounds/fetch.wav");
    audio.loadSoundEffect("footSteps","../resources/sounds/footSteps.wav");
    audio.loadSoundEffect("gameFail","../resources/sounds/gameFail.wav");
    audio.loadSoundEffect("gameVictory","../resources/sounds/gameVictory.wav");
    audio.loadSoundEffect("harp","../resources/sounds/harp.wav");
    audio.loadSoundEffect("hint","../resources/sounds/hint.wav");
    audio.loadSoundEffect("iceSlimeATKClose","../resources/sounds/iceSlimeATKClose.wav");
    audio.loadSoundEffect("iceSlimeATKFur","../resources/sounds/iceSlimeATKFur.wav");
    audio.loadSoundEffect("iceSlimeDie","../resources/sounds/iceSlimeDie.wav");
    audio.loadSoundEffect("idleSing","../resources/sounds/idleSing.wav");
    audio.loadSoundEffect("jump","../resources/sounds/jump.wav");
    audio.loadSoundEffect("push","../resources/sounds/push.wav");
    audio.loadSoundEffect("select","../resources/sounds/select.wav");
    audio.loadSoundEffect("spySlimeDie","../resources/sounds/spySlimeDie.wav");
    audio.loadSoundEffect("successToBuy","../resources/sounds/successToBuy.wav");
    audio.loadSoundEffect("trumpette","../resources/sounds/trumpette.wav");


    // 开始随机播放音乐
    audio.playRandomMusic();
 }

void MenuState::handleInput(RenderWindow& window){
    Event event;
    while(window.pollEvent(event)){
        if(event.type==Event::Closed){
            window.close();
        }
        if (event.type == Event::Resized) {
            cout<<"re!"<<endl;
        }
        if(event.type == Event::MouseButtonPressed){
            if(startButton.isPressed(Mouse::getPosition(window))){
                gameLoad=0;
                game->changeState(make_unique<LevelState>(game));

            }
            if(oldgameButton.isPressed(Mouse::getPosition(window))) {
                gameLoad=1;
                game->changeState(make_unique<LevelState>(game));
            }
            if(exitButton.isPressed(Mouse::getPosition(window))) {
                cout<<"ExitButton clicked!"<<endl;
                window.close();
            }
        }
        
    }
}
void MenuState::update(){
}//更新菜单状态

void MenuState::render(RenderWindow& window){
    window.clear(Color::Black);
    background.draw(window);
    startButton.draw(window);
    exitButton.draw(window);
    oldgameButton.draw(window);  
    window.display();
}
