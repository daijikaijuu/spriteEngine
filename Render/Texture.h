#pragma once

#include "..\homework02.h"

class Texture
{
public:
    Texture();
    virtual ~Texture();

    void LoadTexture(GLuint width, GLuint height, const GLvoid *imageData);

    void BindTexture(GLuint textureUnit = 0);
    void setFiltering();

private:
    GLuint m_textureID;
    GLuint m_Sampler;
};

