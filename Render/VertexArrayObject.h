#pragma once

#include "../homework02.h"
#include "../Logger.h"
#include "Vertex.h"
#include "Shader.h"

class VertexBufferObject;

class VertexArrayObject : public Logger
{
public:
    VertexArrayObject();
    virtual ~VertexArrayObject();

    void Generate(GLuint attribute, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer);

    void Bind() const;
    void UnBind() const;

    GLuint GetID() const;
    VertexBufferObject* GetVBO() const;

    virtual std::string class_type() { return "VertexArrayObject"; }

private:
    GLuint m_VAO;
    VertexBufferObject *m_VBO;
};
