#include "../include/EndState.h"
#include "../include/MenuState.h"
#include "../include/Game.h"
#include "../include/AudioManager.h"
EndState::EndState(Game* game) : game(game) , subtitleYPos(-50.f), subtitleSpeed(100.f){
    //加载字体
    if (!font.loadFromFile("../resources/fonts/yuanshen.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    //设置文本
    subtitle.setFont(font);
    string s="Made  by  :  Mechadrug \n Press Enter to Restart";
    subtitle.setString(s);
    subtitle.setCharacterSize(30);
    subtitle.setFillColor(sf::Color::White);
    subtitle.setPosition(100.f, subtitleYPos);
    time=false;
}

void EndState::handleInput(RenderWindow& window) {
    if(!time){
        time=true;
        if(getCat){
            audio.playSpecificMusic("tail",true);
        }else{
            audio.stopMusic();
            audio.playSoundEffect(SoundChannel::System,"gameFail",SoundPriority::HIGH);
        }
    }

    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        }
        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::Enter) {
                game->changeState(make_unique<MenuState>(game));
            }
        }
    }
}

void EndState::update() {
    //更新结算界面逻辑

    float deltaTime=clock.restart().asSeconds();
    subtitleYPos += subtitleSpeed * deltaTime;
    subtitle.setPosition(400.f, subtitleYPos);
}

void EndState::render(sf::RenderWindow& window) {
    window.clear();
    //绘制结算界面内容
    window.draw(subtitle);
    window.display();
}
