#pragma once

#include "../homework02.h"
#include "../Logger.h"

class Texture : public Logger
{
public:
    Texture(GLsizei items = 1);
    virtual ~Texture();

    void LoadTexture(GLuint width, GLuint height, const GLvoid *imageData);
    void LoadPNGTexture(const std::string & filename);

    void BindTexture(GLuint textureUnit = 0);
    void setFiltering();

    GLuint GetID() const;
    GLsizei GetItems() const;

    virtual std::string class_type() { return "Texture"; }

private:
    GLuint m_textureID;
    GLuint m_Sampler;
    GLsizei m_items;
};

