#include "../include/EndState.h"
#include "../include/MenuState.h"
#include "../include/Game.h"

EndState::EndState(Game* game) : game(game) {}

void EndState::handleInput(RenderWindow& window) {
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
    // 更新结算界面逻辑
}

void EndState::render(sf::RenderWindow& window) {
    window.clear();
    // 绘制结算界面内容
    window.display();
}
