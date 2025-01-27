#include "TextureLoader.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
using namespace std;

unordered_map<string, sf::Texture> TextureMaker::textureMap;

void TextureMaker::LoadTexture(string fileName) //Loads textures from the images folder
{
    string temp = "files/images/"; // or "images/"
    temp.append(fileName);
    temp.append(".png");
    textureMap[fileName].loadFromFile(temp); //Adds the texture by filename to the unordered_map
}

sf::Texture& TextureMaker::GetTexture(string textureName)
{
    // if the texture DOESN'T exist...
    if (textureMap.find(textureName) == textureMap.end()) // we didn't find it
        LoadTexture(textureName);

    // load it first? Then return it?
    return textureMap[textureName];

}

void TextureMaker::Clear()
{
    textureMap.clear(); // get rid of all stored objects in within the map
}
