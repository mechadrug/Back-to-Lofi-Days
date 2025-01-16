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
#include "../include/AudioManager.h"
#include <iostream>
#include <SFML/System.hpp> // 包含 SFML 的系统头文件

// 辅助函数：将 sf::Sound::Status 转换为字符串
std::string soundStatusToString(sf::Sound::Status status) {
    switch (status) {
        case sf::Sound::Stopped: return "Stopped";
        case sf::Sound::Paused: return "Paused";
        case sf::Sound::Playing: return "Playing";
        default: return "Unknown";
    }
}

int main() {
    AudioManager& audio = AudioManager::getInstance();

    // 加载音效
    if (!audio.loadSoundEffect("bloodUp", "../resources/sounds/bloodUp.wav")) {
        std::cerr << "Failed to load sound effect 'bloodUp'." << std::endl;
        return -1;
    }
    if (!audio.loadSoundEffect("jump", "../resources/sounds/jump.wav")) {
        std::cerr << "Failed to load sound effect 'jump'." << std::endl;
        return -1;
    }
    if (!audio.loadSoundEffect("attack", "../resources/sounds/attack.wav")) {
        std::cerr << "Failed to load sound effect 'attack'." << std::endl;
        return -1;
    }
    if (!audio.loadSoundEffect("click", "../resources/sounds/push.wav")) {
        std::cerr << "Failed to load sound effect 'click'." << std::endl;
        return -1;
    }

    // 加载音乐
    std::vector<std::pair<std::string, std::string>> musicFiles = {
        {"battle", "../resources/sounds/music/game1.mp3"},
        {"explosion_music", "../resources/sounds/music/game2.mp3"},
        {"water_music", "../resources/sounds/music/game3.mp3"},
        {"death_music", "../resources/sounds/music/game4.mp3"}
    };
    audio.loadMusicFiles(musicFiles);

    // 播放随机音乐
    audio.playRandomMusic();

    std::cout << "Press 'j' to jump, 'a' to attack, 'c' to click, 'b' to play 'bloodUp' sound." << std::endl;
    std::cout << "Press 'q' to quit." << std::endl;

    char input;
    while (std::cin >> input) {
        switch (input) {
            case 'j':
                audio.playSoundEffect(SoundChannel::Player, "jump", SoundPriority::MEDIUM);
                break;
            case 'a':
                audio.playSoundEffect(SoundChannel::Player, "attack", SoundPriority::HIGH);
                break;
            case 'c':
                audio.playSoundEffect(SoundChannel::System, "click", SoundPriority::MEDIUM);
                break;
            case 'b':
                audio.playSoundEffect(SoundChannel::Monster, "bloodUp", SoundPriority::HIGH);
                break;
            case 'q':
                std::cout << "Quitting test." << std::endl;
                return 0;
            default:
                std::cout << "Unknown input. Press 'j' to jump, 'a' to attack, 'c' to click, 'b' to play 'bloodUp' sound, 'q' to quit." << std::endl;
        }
    }

    return 0;
}

