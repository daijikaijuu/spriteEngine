#pragma once

#include "../helpers.h"
#include "../Logger.h"
#include "../homework02.h"
#include <map>

class Texture;

class TextureManager : public Singleton<TextureManager>, public Logger
{
    friend Singleton<TextureManager>;

    using texturesMap = std::map<string, Texture *>;
private:
    TextureManager() :
        Logger(),
        m_textures()
    {}

    ~TextureManager();

public:
    Texture* GetTexture(string filename, GLsizei items = 1);
    Texture* GenerateTexture(const string& textureName);

    virtual string class_type() const { return "TextureManager"; }

private:
    texturesMap m_textures;
};
