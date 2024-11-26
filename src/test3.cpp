// #include <SFML/Graphics.hpp>

// const unsigned int windowWidth = 800;
// const unsigned int windowHeight = 600;

// const unsigned int logicalWidth = 320;  // 逻辑分辨率
// const unsigned int logicalHeight = 240;

// int main()
// {
//     // 创建SFML窗口
//     sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Pixel Scaling Example");

//     // 创建一个简单的矩形作为测试图像
//     sf::RectangleShape shape(sf::Vector2f(50, 50));
//     shape.setFillColor(sf::Color::Green);
//     shape.setPosition(100, 100);  // 设置矩形的位置

//     // 设置纹理缩放因子（例如 2x 缩放）
//     float scaleFactorX = static_cast<float>(windowWidth) / logicalWidth;
//     float scaleFactorY = static_cast<float>(windowHeight) / logicalHeight;

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         // 清空窗口
//         window.clear(sf::Color::Black);

//         // 设置视口，应用缩放
//         window.setView(sf::View(sf::FloatRect(0, 0, logicalWidth, logicalHeight)));

//         // 绘制矩形，图形不会因为窗口的大小改变而失真
//         window.draw(shape);

//         // 显示窗口内容
//         window.display();
//     }

//     return 0;
// }
// #include <SFML/Graphics.hpp>

// const unsigned int windowWidth = 800;
// const unsigned int windowHeight = 600;

// const unsigned int logicalWidth = 320;  // 逻辑分辨率
// const unsigned int logicalHeight = 240;

// int main()
// {
//     // 创建SFML窗口
//     sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "No Pixel Scaling Example");

//     // 创建一个简单的矩形作为测试图像
//     sf::RectangleShape shape(sf::Vector2f(50, 50));
//     shape.setFillColor(sf::Color::Green);
//     shape.setPosition(100, 100);  // 设置矩形的位置

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         // 清空窗口
//         window.clear(sf::Color::Black);

//         // // 计算当前窗口的缩放因子
//         // float scaleFactorX = static_cast<float>(windowWidth) / logicalWidth;
//         // float scaleFactorY = static_cast<float>(windowHeight) / logicalHeight;

//         // // 改变矩形的尺寸，使其适应窗口缩放
//         // shape.setSize(sf::Vector2f(50 * scaleFactorX, 50 * scaleFactorY));

//         // 绘制矩形，图形会因为窗口的大小改变而被拉伸或压缩
//         window.draw(shape);

//         // 显示窗口内容
//         window.display();
//     }

//     return 0;
// }
#include <SFML/Graphics.hpp>

int main()
{
    // 获取屏幕的分辨率
    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned int screenHeight = desktopMode.height;
    unsigned int screenWidth = desktopMode.width;

    // 计算窗口的高度为屏幕高度的 0.8
    unsigned int windowHeight = static_cast<unsigned int>(screenHeight * 0.8);

    // 根据 4:3 比例计算窗口宽度
    unsigned int windowWidth = windowHeight * 4 / 3;

    // 创建SFML窗口，禁用调整窗口大小
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Fixed Aspect Ratio Window");//, sf::Style::Titlebar | sf::Style::Close
    sf::Texture texture;
    if(!texture.loadFromFile("D:/vscode/codes/Back-to-Lofi-Days/resources/images/realbg-menu.png")){
        return -1;
    };
    // 获取逻辑分辨率 (例如 320x240)
    const unsigned int logicalWidth = 320;
    const unsigned int logicalHeight = 240;

    // 计算窗口与逻辑分辨率的缩放因子
    float scaleFactorX = static_cast<float>(windowWidth) / logicalWidth;
    float scaleFactorY = static_cast<float>(windowHeight) / logicalHeight;

    // 创建一个简单的矩形作为测试图像
    sf::RectangleShape shape(sf::Vector2f(50, 50));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(100, 100);  // 设置矩形的位置


    sf::Sprite sprite(texture);
     sf::Vector2u windowSize = window.getSize();

    // 获取精灵图像的原始尺寸
    sf::Vector2u textureSize = texture.getSize();

    // 计算缩放因子
    float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
    float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);

    // 应用缩放，使得精灵图像填充整个窗口
    sprite.setScale(scaleX, scaleY);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 清空窗口
        window.clear(sf::Color::White);

        // 设置视口，保持 4:3 的比例，并进行像素缩放
        //window.setView(sf::View(sf::FloatRect(0, 0, logicalWidth, logicalHeight)));

        // 绘制矩形
        window.draw(sprite);

        // 显示窗口内容
        window.display();
    }

    return 0;
}
// #include <SFML/Graphics.hpp>

// int main()
// {
//     // 获取屏幕的分辨率
//     const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
//     unsigned int screenHeight = desktopMode.height;
//     unsigned int screenWidth = desktopMode.width;

//     // 计算窗口的高度为屏幕高度的 0.8
//     unsigned int windowHeight = static_cast<unsigned int>(screenHeight * 0.8);

//     // 根据 4:3 比例计算窗口宽度
//     unsigned int windowWidth = windowHeight * 4 / 3;

//     // 创建SFML窗口，禁用调整窗口大小
//     sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Fixed Aspect Ratio Window", sf::Style::Titlebar | sf::Style::Close);

//     // 获取逻辑分辨率 (例如 320x240)
//     const unsigned int logicalWidth = 800;
//     const unsigned int logicalHeight = 600;

//     // 计算窗口与逻辑分辨率的缩放因子
//     float scaleFactorX = static_cast<float>(windowWidth) / logicalWidth;
//     float scaleFactorY = static_cast<float>(windowHeight) / logicalHeight;

//     // 创建一个简单的矩形作为测试图像
//     sf::RectangleShape shape(sf::Vector2f(50, 50));
//     shape.setFillColor(sf::Color::Green);
//     shape.setPosition(100, 100);  // 设置矩形的位置

//     // 创建视口来设置缩放
//     sf::View view(sf::FloatRect(0, 0, logicalWidth, logicalHeight));

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         // 清空窗口
//         window.clear(sf::Color::Black);

//         // 设置视口的缩放因子，保持4:3比例并进行像素缩放
//         view.setSize(logicalWidth * scaleFactorX, logicalHeight * scaleFactorY);
//         window.setView(view);

//         // 绘制矩形
//         window.draw(shape);

//         // 显示窗口内容
//         window.display();
//     }

//     return 0;
// }


