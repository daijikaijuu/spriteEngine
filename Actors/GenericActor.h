#pragma once

#include "../homework02.h"
#include "../Render/Shader.h"
#include "../Render/Texture.h"
#include "../Render/VertexArrayObject.h"
#include "../Render/VertexBufferObject.h"

class GenericActor
{
public:
    GenericActor(GLfloat x, GLfloat y, GLfloat size, GLfloat z = 0.0f);
    virtual ~GenericActor();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    virtual void Move(GLfloat xShift, GLfloat yShift);

    inline GLfloat X() { return m_x; }
    inline GLfloat Y() { return m_y; }
    inline GLfloat Z() { return m_z; }

protected:
    GLfloat m_x, m_y, m_z;
    GLfloat m_size;
    GLint m_elapsedTime;

    Shader *m_shader;
    VertexArrayObject *m_VAO;
};

class TexturedActor : public GenericActor
{
public:
    TexturedActor(GLfloat x, GLfloat y, GLfloat size, GLfloat z = 0.0f);
    virtual ~TexturedActor();

    virtual void Draw();

protected:
    Texture *m_texture;
};
