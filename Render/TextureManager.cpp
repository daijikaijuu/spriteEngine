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
#ifdef _DEBUG
            switch (e)
            {
            case 1:
                std::cout << "Failed to open file: " << filename.c_str() << std::endl;
                break;
            default:
                std::cout << "Exception #" << e << std::endl;
                break;
            }
#endif // _DEBUG
        }
        m_textures.insert(std::pair<std::string, Texture*>(filename, result));
    }
    else
        result = it->second;

    return result;
}

