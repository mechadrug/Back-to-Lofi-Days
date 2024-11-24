#include "../include/Button.h"

Button::Button(float x,float y,float width,float height,const Font&font,const string&text){
    shape.setPosition(x,y);//按钮的基点位置
    shape.setSize({width,height});//按钮的大小
    shape.setFillColor(Color::Blue);//按钮的颜色

    label.setFont(font);//文字的字体
    label.setString(text);//文本
    label.setCharacterSize(24);//字号
    label.setFillColor(Color::White);//字色
    label.setPosition(x+20,y+10);//字基点位置

}

void Button::draw(RenderWindow& window){
    window.draw(shape);
    window.draw(label);
    //把按钮画到当前渲染对象的窗口
}

bool Button::isClicked(const Event::MouseButtonEvent& mouseEvent){
    return shape.getGlobalBounds().contains(mouseEvent.x,mouseEvent.y);
}//检测是否按了按钮

