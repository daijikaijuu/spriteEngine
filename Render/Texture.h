#pragma once

#include "../homework02.h"

class Texture
{
public:
    Texture(GLsizei items = 1);
    virtual ~Texture();

    void LoadTexture(GLuint width, GLuint height, const GLvoid *imageData);
    void LoadPNGTexture(const std::string filename);

    void BindTexture(GLuint textureUnit = 0);
    void setFiltering();

    GLuint GetID() const;
    GLsizei GetItems() const;

private:
    GLuint m_textureID;
    GLuint m_Sampler;
    GLsizei m_items;
};

