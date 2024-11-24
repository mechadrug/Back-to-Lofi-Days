#ifndef BUTTON_H
#define BUTTON_H

#include<SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Button{
    private:
    RectangleShape shape;
    Text label;
    public:
    Button(float x,float y,float width,float height,const Font& font,const string& text);//构造函数
    void draw(RenderWindow& window);//
    bool isClicked(const Event::MouseButtonEvent&mouseEvent);//处理点击事件

};


#endif