#ifndef MAP_H
#define MAP_H

#include "../SFML/Graphics.hpp"
#include <memory>
#include <stdexcept>
#include "Game.h"
#include<iostream>
using namespace std;
using namespace sf;
class Map{
    private:
     Sprite bgSprite;
     float scaleX;
     float scaleY;
    public:
    Map()=default;
    Map(const string& bgFile);
    void draw(RenderWindow&window);
    float returnScaleX();
    float returnScaleY();
};

#endif