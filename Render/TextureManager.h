#pragma once

#include "../helpers.h"
#include "../homework02.h"
#include <map>

class Texture;

class TextureManager : public Singleton<TextureManager>
{
    friend Singleton<TextureManager>;

    using texturesMap = std::map<std::string, Texture *>;
private:
    TextureManager() :
        m_textures()
    {}

    ~TextureManager();

public:
    Texture* GetTexture(std::string filename);

private:
    texturesMap m_textures;
};
