#include <SFML/Graphics.hpp>
#include <memory>
#include <stdexcept>
#include "Game.h"
#include<iostream>
using namespace std;
using namespace sf;
class Map{
    private:
     shared_ptr<Texture> bgTexture;
     Sprite bgSprite;
    public:
    Map(const string& bgFile);
    void draw(RenderWindow&window);
};