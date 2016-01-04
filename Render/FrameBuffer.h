#pragma once

#include "../homework02.h"
#include "../helpers.h"

class Texture;
class VertexArrayObject;
class Shader;

class FrameBuffer : public Singleton<FrameBuffer>
{
    friend Singleton<FrameBuffer>;
public:
    FrameBuffer();
    virtual ~FrameBuffer();

    void Resize(GLsizei width, GLsizei height);

    void Bind();
    void UnBind();
    void Render();

    void AttachTexture(Texture *texture);

private:
    GLuint m_ID, m_depthBuffer;
    GLuint m_width, m_height;
    Texture *m_texture;
    VertexArrayObject *m_VAO;
    Shader *m_shader;
};

