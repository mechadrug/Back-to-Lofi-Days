#include "../include/TexturePool.h"

unordered_map<std::string, sf::Texture> TexturePool::textures;

Texture& TexturePool::getTexture(const string& fileName){
    // 查找纹理缓存中是否已有该纹理
    auto it = textures.find(fileName);
    if (it != textures.end()) {
        // 如果找到了，返回已加载的纹理
        return it->second;
    }

    // 如果没有，加载纹理并缓存
    Texture texture;
    if (!texture.loadFromFile(fileName)) {
        cerr << "Error loading texture: " << fileName << endl;
        throw runtime_error("Failed to load texture");
    }

    // 将新纹理加入缓存
    textures[fileName] = move(texture);
    return textures[fileName];
}

void TexturePool::clear() {
    textures.clear();  // 清空纹理缓存
}