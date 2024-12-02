#include <SFML/Graphics.hpp>
#include <iostream>

// 创建编号map，方便显示所按键的名称
std::map<int, std::string> bt_name {
   {0, "A"},
   {1, "B"},
   {3, "X"},
   {4, "Y"},
   {6, "L1"},
   {7, "R1"},
   {8, "L2"},
   {9, "R2"},
   {10, "Return"},
   {11, "Menu"},
   {13, "Left Stick"}, 
   {14, "Right Stick"},
   {20, "MI"}
};

int main() {
   int Width(1080), Height(720);
   sf::RenderWindow window(sf::VideoMode(Width, Height, 32), "Joystick Demo");
   
   sf::Font font;
   font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
   sf::Text content;
   content.setFont(font);
   content.setStyle(sf::Text::Bold);
   content.setFillColor(sf::Color::White);
   content.setPosition(Width/2, Height/2);
   float ox, oy;

   while (window.isOpen()) {
      sf::Event event;
      std::string str;
      while (window.pollEvent(event)) {
         if (event.type==sf::Event::Closed) window.close();
         else if (event.type==sf::Event::JoystickMoved) {
            // event.joystickMove.axis返回沿哪个坐标轴移动的坐标轴编号
            sf::Joystick::Axis axis=event.joystickMove.axis;
            // event.joystickMove.position返回摇杆移动的值，范围为[-100, 100]
            int pos=(int)event.joystickMove.position;
            int x,y;
            std::string stk;
            axis%2==0?x=pos:y=pos;

            if (axis==0 || axis==1) stk="Left Stick: ";
            else if (axis==2 || axis==3) stk="Right Stick: ";
            else if (axis==6 || axis==7) stk="Arrow Key: "; 
            str=stk+std::to_string(x)+", "+std::to_string(y);
            content.setString(str);
            content.setCharacterSize(72);
            ox=content.getLocalBounds().left+content.getLocalBounds().width/2;
            oy=content.getLocalBounds().top+content.getLocalBounds().height/2;
            content.setOrigin(ox, oy);
            window.draw(content);
         }
         else if (event.type==sf::Event::JoystickButtonPressed) {
            // enent.joystickButton.button返回按下的游戏手柄按键的编号
            str=bt_name[event.joystickButton.button];
            content.setString(str);
            content.setCharacterSize(96);
            ox=content.getLocalBounds().left+content.getLocalBounds().width/2;
            oy=content.getLocalBounds().top+content.getLocalBounds().height/2;
            content.setOrigin(ox, oy);
            window.draw(content);
         }
         window.setActive();
         window.display();
         window.clear();
      }
   }
   return 0;
}