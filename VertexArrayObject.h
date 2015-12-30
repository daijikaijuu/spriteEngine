#pragma once

#include "homework02.h"

class VertexBufferObject;

class VertexArrayObject
{
public:
    VertexArrayObject();
    virtual ~VertexArrayObject();

    void Generate(GLuint attribute, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer);

    void Bind() const;
    void UnBind() const;

    GLuint GetID() const;
    VertexBufferObject* GetVBO() const;

private:
    GLuint m_VAO;
    VertexBufferObject *m_VBO;
};
