// #include <SFML/Graphics.hpp>

// int main() {
//     // 创建一个窗口
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Border Example");

//     // 创建一个矩形，覆盖整个窗口
//     sf::RectangleShape border(sf::Vector2f(window.getSize().x, window.getSize().y));
//     border.setFillColor(sf::Color::Transparent);  // 设置透明填充
//     border.setOutlineThickness(10);  // 设置边框厚度
//     border.setOutlineColor(sf::Color(255, 255, 255));  // 设置边框颜色为白色

//     // 创建一个简单的背景矩形，用来模拟游戏区域
//     sf::RectangleShape background(sf::Vector2f(window.getSize().x - 20, window.getSize().y - 20));  // 留出边框的空间
//     background.setFillColor(sf::Color(100, 150, 255));  // 背景色：浅蓝色
//     background.setPosition(10, 10);  // 偏移以留出边框空间

//     // 游戏循环
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         // 清空窗口
//         window.clear();

//         // 绘制边框
//         window.draw(border);

//         // 绘制游戏内容区域（例如背景）
//         window.draw(background);

//         // 显示内容
//         window.display();
//     }

//     return 0;
// }
