#pragma once

#include "../homework02.h"
#include "../Logger.h"

class Texture : public Logger
{
public:
    Texture(GLsizei items = 1);
    virtual ~Texture();

    void LoadTexture(GLuint width, GLuint height, const GLvoid *imageData);
    void LoadPNGTexture(const string &filename);

    void BindTexture(GLuint textureUnit = 0);
    void setFiltering();

    GLuint GetID() const;
    GLsizei GetItems() const;

    Texture& operator=(const Texture &texture) = delete;
    Texture(const Texture &texture) = delete;

    virtual string class_type() const { return "Texture"; }
    virtual bool DEBUG_OK() const;
    virtual string DEBUG_DUMP() const;

private:
    GLuint m_textureID;
    GLuint m_Sampler;
    GLsizei m_items;
};

