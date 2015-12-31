#pragma once

#include "../helpers.h"
#include "../homework02.h"
#include <map>

class Texture;

class TextureManager
{
    using texturesMap = std::map<std::string, Texture *>;
private:
    TextureManager() :
        m_textures()
    {}
    TextureManager(TextureManager &) {}
    void operator=(TextureManager &) {}

    ~TextureManager();

    static TextureManager *m_instance;

public:
    static TextureManager* get_instance()
    {
        if (m_instance == NULL)
            m_instance = new TextureManager();
        return static_cast<TextureManager *>(m_instance);
    }

    static void Kill()
    {
        if (m_instance != NULL)
        {
            delete m_instance;
            m_instance = NULL;
        }
    }

    Texture* GetTexture(std::string filename);

private:
    texturesMap m_textures;
};
