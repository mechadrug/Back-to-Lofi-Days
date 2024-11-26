#include "../include/Map.h"

Map::Map(const string &bgFile){
    bgTexture=make_shared<Texture>();
    if(!bgTexture->loadFromFile(bgFile)){
        throw runtime_error("Failed to load background texture!");
    }
    bgSprite.setTexture(*bgTexture);
}
void Map::draw(RenderWindow& window){
    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned int screenHeight = desktopMode.height;
    unsigned int screenWidth = desktopMode.width;
    unsigned int windowHeight = static_cast<unsigned int>(screenHeight * 0.8);
    unsigned int windowWidth = windowHeight * 4 / 3;
    float scaleX = static_cast<float>(windowWidth) / static_cast<float>(bgTexture->getSize().x);
    float scaleY = static_cast<float>(windowHeight) / static_cast<float>(bgTexture->getSize().y);
    bgSprite.setScale(scaleX, scaleY);
    window.draw(bgSprite);

}