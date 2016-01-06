#include "../homework02.h"
#include "../Render/Texture.h"
#include "../Render/TextureManager.h"
#include "../Render/Vertex.h"
#include "../Render/Shader.h"
#include "../Render/FrameBuffer.h"
#include "ActorAmbientLight.h"

ActorAmbientLight::ActorAmbientLight(GLfloat x, GLfloat y, GLfloat z, GLfloat size, glm::vec3 color) :
    TexturedActor(x, y, size, z),
    m_color(color)
{
    m_shader->Load("Data/Shaders/light");
    m_texture = TextureManager::GetInstance()->GetTexture("Data/Textures/light.png");

    ShapeData<TexturedVertex> *vertexData = shapeGenerator::generateTexturedQuad(0, 0, m_z, 400, 300);

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(vertexData->vertices, vertexData->vertexBufferSize());
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    BindShaderAttributesAndUniforms();

    m_VAO->Generate<TexturedVertex>(m_shader, vertexData, "inPosition", 0);
    m_VAO->Generate<TexturedVertex>(m_shader, vertexData, "inCoord", 1);

    UpdateMVP();

    delete vertexData;
}

ActorAmbientLight::~ActorAmbientLight()
{
}

void ActorAmbientLight::Draw()
{
    TexturedActor::Draw();

    FrameBuffer::GetInstance()->GetTexture()->BindTexture(1);
    m_shader->Bind();
    glDrawArrays(GL_QUADS, 0, 4);
    m_shader->UnBind();
}

void ActorAmbientLight::ResizeScene(GLsizei width, GLsizei height)
{
    TexturedActor::ResizeScene(width, height);

    GLint projection = m_shader->GetUniformLocation("projection");
    if (projection != -1)
    {
        glm::mat4 p = glm::ortho(0.0f, 1.0f * width, 1.0f * height, 0.0f);
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
    }
    glUniform2fv(m_shader->GetUniformLocation("resolution"), 1, glm::value_ptr(glm::vec2(width, height)));
}

void ActorAmbientLight::BindShaderAttributesAndUniforms()
{
    TexturedActor::BindShaderAttributesAndUniforms();

    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
    m_shader->RegisterUniform({ "gSampler", "lightColor", "lightmap", "resolution" });

    glUniform1i(m_shader->GetUniformLocation("gSampler"), 1);
    glUniform3fv(m_shader->GetUniformLocation("lightColor"), 1, glm::value_ptr(m_color));
    glUniform1i(m_shader->GetUniformLocation("lightmap"), 0);
}
