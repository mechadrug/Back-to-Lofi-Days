#include "../include/Button.h"
#include<iostream>
Button::Button(float x,float y,float width,float height,const Font&font,const string&text,
Color bgColor,Color textColor){
    shape.setPosition(x,y);//按钮的基点位置
    shape.setSize({width,height});//按钮的大小
    shape.setFillColor(bgColor);//按钮的颜色
    shape.setOutlineThickness(1.0f);//默认边框厚度
    shape.setOutlineColor(Color::White);//默认边框是白色的

    label.setFont(font);//文字的字体
    label.setString(text);//文本
    label.setCharacterSize(static_cast<unsigned int>(height * 0.5));
    label.setFillColor(textColor);//字色
    sf::FloatRect labelBounds = label.getLocalBounds();
    label.setPosition(
        x + (width - labelBounds.width) / 2 - labelBounds.left,
        y + (height - labelBounds.height) / 2 - labelBounds.top
    );//字位于中心

}
/*_____able to change private member through these functions_____*/
void Button::draw(RenderWindow& window){
    window.draw(shape);
    window.draw(label);
    //把按钮画到当前渲染对象的窗口
}

bool Button::isClicked(const Event::MouseButtonEvent& mouseEvent){
    return shape.getGlobalBounds().contains(mouseEvent.x,mouseEvent.y);
}//检测是否按了按钮
void Button:: setTextColor(const Color& color){
    label.setFillColor(color);
}//文字颜色
void Button::setOutlineThickness(float thickness){
    shape.setOutlineThickness(thickness);
}//边框厚度
void Button::setOutlineColor(const Color& color){
    shape.setOutlineColor(color);
}//边框颜色
void Button::setBackgroundColor(const Color&color){
    shape.setFillColor(color);
}//按钮的整体颜色

void Button::updateHover(const Vector2f& mousePosition) {
    if (shape.getGlobalBounds().contains(mousePosition)) {
        shape.setFillColor(Color(100, 100, 100, 128)); //悬停颜色
    } else {
        shape.setFillColor(Color::Blue);       //默认颜色
    }
}

