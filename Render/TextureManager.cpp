#include "Texture.h"
#include "TextureManager.h"

TextureManager * TextureManager::m_instance = NULL;

TextureManager::~TextureManager()
{
    for (auto &it : m_textures)
    {
        delete it.second;
    }
    m_textures.clear();
}


Texture* TextureManager::GetTexture(std::string filename)
{
    Texture *result = NULL;
    texturesMap::iterator it = m_textures.find(filename);
    if (it == m_textures.end())
    {
        result = new Texture();
        if (!result->LoadPNGTexture(filename))
        {
            delete result;
            return NULL;
        }
        m_textures.insert(std::pair<std::string, Texture*>(filename, result));
    }
    else
        result = it->second;

    return result;
}
