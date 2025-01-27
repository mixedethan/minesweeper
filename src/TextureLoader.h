#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <string>

using namespace std;

class TextureMaker {
   static unordered_map<string, sf::Texture> textureMap; //all texture maker objects share this
   static void LoadTexture(string textureName);
public:
    static sf::Texture& GetTexture(string textureName);
    static void Clear(); // Call at the end of main()
};