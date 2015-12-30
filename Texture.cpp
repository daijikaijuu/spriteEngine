#include "Texture.h"

Texture::Texture() :
    m_textureID(0)
{
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glGenSamplers(1, &m_Sampler);
}


Texture::~Texture()
{
    glDeleteSamplers(1, &m_Sampler);
    glDeleteTextures(1, &m_textureID);
}

void Texture::LoadTexture(GLuint width, GLuint height, const GLvoid *imageData)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
}

void Texture::BindTexture(GLuint textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glBindSampler(textureUnit, m_Sampler);
}

void Texture::setFiltering()
{
    glSamplerParameteri(m_Sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_Sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
}