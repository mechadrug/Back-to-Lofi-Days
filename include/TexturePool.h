#ifndef TEXTUREPOOL_H
#define TEXTUREPOOL_H


#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

using namespace std;
using namespace sf;

class TexturePool{
    private:
    static std::unordered_map<std::string, sf::Texture> textures;
    public:
    static Texture& getTexture(const string& fileName);
    static void clear();

};

#endif 
