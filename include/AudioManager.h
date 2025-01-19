#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory>
#include <algorithm>
#include <random>
#include <vector>

// 定义音效优先级
enum class SoundPriority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2
};

// 定义音效通道
enum class SoundChannel {
    Player,
    Monster,
    System
};

// 结构体用于存储每个通道的音效实例
struct SoundChannelInstance {
    sf::Sound sound;
    std::string currentSoundName;
    SoundPriority currentPriority;

    SoundChannelInstance() : currentSoundName(""), currentPriority(SoundPriority::LOW) {}
};

class AudioManager {
public:
    // 获取单例实例
    static AudioManager& getInstance();

    // 删除拷贝构造函数和赋值操作符，确保单例唯一性
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // 加载音效
    bool loadSoundEffect(const std::string& name, const std::string& filename);

    // 在指定通道播放音效
    void playSoundEffect(SoundChannel channel, const std::string& name, SoundPriority priority = SoundPriority::MEDIUM);

    // 加载音乐
    bool loadMusic(const std::string& name, const std::string& filename);

    // 批量加载音乐文件
    void loadMusicFiles(const std::vector<std::pair<std::string, std::string>>& musicFiles);

    // 播放随机音乐
    void playRandomMusic();

    // 播放特定音乐
    void playSpecificMusic(const std::string& name, bool loop = true);

    // 停止当前播放的音乐
    void stopMusic();

    // 设置音效音量（0-100）
    void setSoundVolume(float volume);

    // 设置音乐音量（0-100）
    void setMusicVolume(float volume);

    // 更新音频管理器，清理已停止播放的音效，并处理音乐播放
    void update();

private:
    // 私有构造函数，确保单例模式
    AudioManager();

    // 辅助函数：将 SoundChannel 转换为字符串（用于调试）
    std::string channelToString(SoundChannel channel) const;

    // 存储音效缓冲区
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBuffers;

    // 存储各个通道的音效实例
    std::map<SoundChannel, SoundChannelInstance> channels;

    // 存储音乐对象
    std::map<std::string, std::unique_ptr<sf::Music>> musics;
    sf::Music* currentMusic;
    float musicVolume;

    // 音效音量
    float soundVolume;

    // 音乐池
    std::vector<std::string> musicPool;
    std::mt19937 rng;
    bool isRandomPlaying;
    std::string specificMusicName;
};

#endif // AUDIOMANAGER_H
