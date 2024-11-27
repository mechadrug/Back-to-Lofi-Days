#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <stdexcept>
#include "Game.h"
#include<iostream>
using namespace std;
using namespace sf;
class Map{
    private:
     Sprite bgSprite;
    public:
    Map()=default;
    Map(const string& bgFile);
    void draw(RenderWindow&window);
};

#endif