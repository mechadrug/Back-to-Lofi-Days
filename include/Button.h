#ifndef BUTTON_H
#define BUTTON_H

#include<SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Button{
    private:
    RectangleShape shape; //矩形区域
    Text label; //按钮的文字
    public:
    Button()=default;
    Button(float x,float y,float width,float height,const Font& font,const string& text,Color bgColor,Color textColor);//构造函数
    void draw(RenderWindow& window);//渲染按钮
    bool isClicked(const Event::MouseButtonEvent&mouseEvent);//处理点击事件
    void setBackgroundColor(const Color&color);//按钮整体颜色
    void setTextColor(const Color& color);//文字颜色
    void setOutlineThickness(float thickness);//边框厚度
    void setOutlineColor(const Color& color);//边框颜色

    void updateHover(const Vector2f& mousePosition) ;//悬停效果更新
    RectangleShape& getShape(){
        return shape;
    }


};


#endif