#pragma once

#include "..\homework02.h"
#include "Vertex.h"

class Shader;
class VertexBufferObject;

class VertexArrayObject
{
public:
    VertexArrayObject();
    virtual ~VertexArrayObject();

    void Generate(GLuint attribute, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer);
    template<typename T>
    void Generate(Shader *shader, ShapeData<T> *vertexData, std::string attribute, GLuint position);

    void Bind() const;
    void UnBind() const;

    GLuint GetID() const;
    VertexBufferObject* GetVBO() const;

private:
    GLuint m_VAO;
    VertexBufferObject *m_VBO;
};

template<typename T>
void VertexArrayObject::Generate(Shader *shader, ShapeData<T> *vertexData, std::string attribute, GLuint position)
{
    Generate(shader->GetAttributeLocation(attribute), 3, GL_FLOAT, GL_FALSE, vertexData->itemSize(), vertexData->position(position));
}
