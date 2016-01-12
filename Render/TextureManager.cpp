#include "../Logger.h"
#include "Texture.h"
#include "TextureManager.h"

TextureManager::~TextureManager()
{
    for (auto &it : m_textures)
    {
        delete it.second;
    }
    m_textures.clear();
}

Texture* TextureManager::GetTexture(std::string filename, GLsizei items)
{
    Texture *result = NULL;
    texturesMap::iterator it = m_textures.find(filename);
    if (it == m_textures.end())
    {
        result = new Texture(items);
        try
        {
            result->LoadPNGTexture(filename);
        }
        catch (int e)
        {
            switch (e)
            {
            case 1:
                Error("Failed to open texture: ", filename.c_str());
                break;
            default:
                Error("Unknown exception #", e);
                break;
            }
        }
        m_textures.insert(std::pair<std::string, Texture*>(filename, result));
    }
    else
        result = it->second;

    return result;
}

