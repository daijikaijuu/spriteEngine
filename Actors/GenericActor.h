#pragma once

#include "../homework02.h"
#include "../Logger.h"
#include "../Render/Shader.h"
#include "../Render/Texture.h"
#include "../Render/VertexArrayObject.h"
#include "../Render/VertexBufferObject.h"

class GenericActor : public Logger
{
public:
    GenericActor(GLfloat x, GLfloat y, GLfloat size, GLfloat z = 0.0f);
    virtual ~GenericActor();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    virtual void Move(GLfloat xShift, GLfloat yShift);
    virtual void MoveTo(GLfloat x, GLfloat y);
    virtual void ResizeScene(GLsizei width, GLsizei height);

    inline GLfloat X() { return m_x; }
    inline GLfloat Y() { return m_y; }
    inline GLfloat Z() { return m_z; }

    virtual string class_type() const { return "GenericActor"; }
    virtual bool DEBUG_OK() const;
    virtual string DEBUG_DUMP() const;

protected:
    GLfloat m_x, m_y, m_z;
    GLsizei m_sceneWidth, m_sceneHeight;
    GLfloat m_size;
    GLint m_elapsedTime;

    GLint m_MVP;
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_projection;

    Shader *m_shader;
    VertexArrayObject *m_VAO;

    virtual void UpdateMVP();
    virtual void BindShaderAttributesAndUniforms();

    void InitializeShader(const string &vertexFilename, const string &fragmentFilename);
    template<typename T> void InitializeVBO(T *vertexData);
    template<typename T> void SetAttribute(T *vertexData, const string attribute, GLuint position);
};

class TexturedActor : public GenericActor
{
public:
    TexturedActor(GLfloat x, GLfloat y, GLfloat size, GLfloat z = 0.0f);
    virtual ~TexturedActor();

    virtual void Draw();

    virtual string class_type() const { return "TexturedActor"; }
    virtual bool DEBUG_OK() const;
    virtual string DEBUG_DUMP() const;

protected:
    Texture *m_texture;

    virtual void BindShaderAttributesAndUniforms();
};

template<typename T>
inline void GenericActor::InitializeVBO(T *vertexData)
{
    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(vertexData->vertices, vertexData->vertexBufferSize());
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);
}

template<typename T>
inline void GenericActor::SetAttribute(T *vertexData, const string attribute, GLuint position)
{
    m_VAO->Generate(m_shader->GetAttributeLocation(attribute), 3, GL_FLOAT, GL_FALSE, vertexData->itemSize(), vertexData->position(position));
}
