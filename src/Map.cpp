#include "../include/Map.h"
#include "../include/TexturePool.h"
Map::Map(const string &bgFile){
    Texture&texture = TexturePool::getTexture(bgFile);
    bgSprite.setTexture(texture);
    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned int screenHeight = desktopMode.height;
    unsigned int screenWidth = desktopMode.width;
    unsigned int windowHeight = static_cast<unsigned int>(screenHeight * 0.8);
    unsigned int windowWidth = windowHeight * 4 / 3;
    scaleX = static_cast<float>(windowWidth) / static_cast<float>(texture.getSize().x);
    scaleY = static_cast<float>(windowHeight) / static_cast<float>(texture.getSize().y);
    bgSprite.setScale(scaleX, scaleY);
    cout<<scaleX<<" "<<scaleY<<endl;

}
void Map::draw(RenderWindow& window){
    window.draw(bgSprite);
}
float Map::returnScaleX(){
    return scaleX;
}
float Map::returnScaleY(){
    return scaleY;
}

