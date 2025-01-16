#include "../include/AudioManager.h"
#include "../include/GlobalVar.h"
#include <iostream>
#include <chrono>


// 辅助函数：将 SoundChannel 转换为字符串
std::string AudioManager::channelToString(SoundChannel channel) const {
    switch(channel) {
        case SoundChannel::Player: return "Player";
        case SoundChannel::Monster: return "Monster";
        case SoundChannel::System: return "System";
        default: return "Unknown";
    }
}

// 获取单例实例
AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

// 私有构造函数
AudioManager::AudioManager()
    : currentMusic(nullptr), musicVolume(50.f), soundVolume(100.f),
      isRandomPlaying(true), specificMusicName("") {
    // 初始化音效通道
    channels[SoundChannel::Player] = SoundChannelInstance();
    channels[SoundChannel::Monster] = SoundChannelInstance();
    channels[SoundChannel::System] = SoundChannelInstance();

    // 初始化随机数生成器
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
}

// 加载音效
bool AudioManager::loadSoundEffect(const std::string& name, const std::string& filename) {
    if (soundBuffers.find(name) != soundBuffers.end()) {
        std::cerr << "Sound effect '" << name << "' is already loaded." << std::endl;
        return false;
    }

    auto buffer = std::make_shared<sf::SoundBuffer>();
    if (!buffer->loadFromFile(filename)) {
        std::cerr << "Failed to load sound effect from file: " << filename << std::endl;
        return false;
    }

    soundBuffers[name] = buffer;
    std::cout << "Loaded sound effect: " << name << " from " << filename << std::endl;
    return true;
}

// 在指定通道播放音效
void AudioManager::playSoundEffect(SoundChannel channel, const std::string& name, SoundPriority priority) {
    auto it = soundBuffers.find(name);
    if (it == soundBuffers.end()) {
        std::cerr << "Sound effect '" << name << "' not found." << std::endl;
        return;
    }

    SoundChannelInstance& targetChannel = channels[channel];

    // 检查目标通道是否正在播放相同的音效
    if (targetChannel.sound.getStatus() == sf::Sound::Playing && targetChannel.currentSoundName == name) {
        std::cout << "Sound effect '" << name << "' is already playing on channel " << channelToString(channel) << ". Ignoring play request." << std::endl;
        return;
    }

    // 检查目标通道当前播放的音效优先级
    if (targetChannel.sound.getStatus() == sf::Sound::Playing) {
        // 比较新音效的优先级与当前播放音效的优先级
        if (static_cast<int>(priority) <= static_cast<int>(targetChannel.currentPriority)) {
            std::cout << "Cannot play sound effect '" << name << "' on channel " << channelToString(channel) 
                      << " because a higher or equal priority sound is already playing." << std::endl;
            return;
        }
        else {
            // 新音效优先级更高，停止当前播放的音效
            std::cout << "Stopping sound effect '" << targetChannel.currentSoundName << "' on channel " 
                      << channelToString(channel) << " to play higher priority sound effect '" << name << "'." << std::endl;
            targetChannel.sound.stop();
            targetChannel.currentSoundName = "";
        }
    }

    // 播放新音效
    targetChannel.sound.setBuffer(*it->second);
    targetChannel.sound.setVolume(soundVolume);
    targetChannel.sound.play();
    targetChannel.currentSoundName = name;
    targetChannel.currentPriority = priority;

    std::cout << "Playing sound effect '" << name << "' on channel " << channelToString(channel) << " with priority " 
              << static_cast<int>(priority) << "." << std::endl;
}

// 加载音乐
bool AudioManager::loadMusic(const std::string& name, const std::string& filename) {
    if (musics.find(name) != musics.end()) {
        std::cerr << "Music '" << name << "' is already loaded." << std::endl;
        return false;
    }

    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(filename)) {
        std::cerr << "Failed to load music from file: " << filename << std::endl;
        return false;
    }

    music->setVolume(musicVolume);
    musics[name] = std::move(music);
    std::cout << "Loaded music: " << name << " from " << filename << std::endl;
    return true;
}

// 批量加载音乐文件
void AudioManager::loadMusicFiles(const std::vector<std::pair<std::string, std::string>>& musicFiles) {
    for (const auto& pair : musicFiles) {
        const std::string& name = pair.first;
        const std::string& path = pair.second;
        if (loadMusic(name, path)) {
            musicPool.push_back(name);
        }
        else {
            std::cerr << "Failed to load music: " << path << std::endl;
        }
    }
}

// 播放随机音乐
void AudioManager::playRandomMusic() {
    if (musicPool.empty()) {
        std::cerr << "Music pool is empty. Cannot play random music." << std::endl;
        return;
    }
    // 随机选择一首音乐
    std::uniform_int_distribution<std::size_t> dist(0, musicPool.size() - 1);
    std::size_t index = dist(rng);
    std::string selectedMusic = musicPool[index];

    std::cout << "Playing random music: " << selectedMusic << std::endl;

    playSpecificMusic(selectedMusic, true);
    isRandomPlaying = true;
    specificMusicName = "";
}

// 播放特定音乐
void AudioManager::playSpecificMusic(const std::string& name, bool loop) {
    auto it = musics.find(name);
    if (it == musics.end()) {
        std::cerr << "Music '" << name << "' not found." << std::endl;
        return;
    }

    if (currentMusic != nullptr && currentMusic->getStatus() == sf::Music::Playing) {
        currentMusic->stop();
        std::cout << "Stopped current music to play '" << name << "'." << std::endl;
    }

    currentMusic = it->second.get();
    currentMusic->setLoop(loop);
    currentMusic->play();
    std::cout << "Playing specific music: " << name << " with loop " << (loop ? "true" : "false") << "." << std::endl;

    isRandomPlaying = false;
    specificMusicName = name;
}

// 停止音乐
void AudioManager::stopMusic() {
    if (currentMusic != nullptr) {
        currentMusic->stop();
        std::cout << "Stopped music." << std::endl;
    }
    isRandomPlaying = false;
    specificMusicName = "";
}

// 设置音效音量
void AudioManager::setSoundVolume(float volume) {
    soundVolume = volume;
    for (auto& [ch, instance] : channels) {
        instance.sound.setVolume(soundVolume);
    }
    std::cout << "Set sound volume to " << soundVolume << "." << std::endl;
}

// 设置音乐音量
void AudioManager::setMusicVolume(float volume) {
    musicVolume = volume;
    if (currentMusic != nullptr) {
        currentMusic->setVolume(musicVolume);
    }
    for (auto& pair : musics) {
        pair.second->setVolume(musicVolume);
    }
    std::cout << "Set music volume to " << musicVolume << "." << std::endl;
}

// 更新音频管理器
void AudioManager::update() {
    // 检查每个通道的音效状态
    for (auto& [ch, instance] : channels) {
        if (instance.sound.getStatus() == sf::Sound::Stopped && !instance.currentSoundName.empty()) {
            std::cout << "Sound effect '" << instance.currentSoundName << "' on channel " << channelToString(ch) << " has stopped." << std::endl;
            instance.currentSoundName = "";
            instance.currentPriority = SoundPriority::LOW;
        }
    }

    // 管理随机音乐的播放
    if (isRandomPlaying && currentMusic != nullptr && currentMusic->getStatus() == sf::Music::Stopped&&c_idx!=3) {
        playRandomMusic();
    }
}
