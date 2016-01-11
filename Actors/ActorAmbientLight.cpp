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

    TexturedShape *vertexData = shapeGenerator::generateTexturedQuad(0, 0, m_z, 400, 300);
    InitializeVBO<TexturedShape>(vertexData);

    BindShaderAttributesAndUniforms();

    SetAttribute<TexturedShape>(m_shader, vertexData, "inPosition", 0);
    SetAttribute<TexturedShape>(m_shader, vertexData, "inCoord", 1);

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

    glUniform2fv(m_shader->GetUniformLocation("resolution"), 1, glm::value_ptr(glm::vec2(width, height)));
}

void ActorAmbientLight::BindShaderAttributesAndUniforms()
{
    TexturedActor::BindShaderAttributesAndUniforms();

    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
    m_shader->RegisterUniform({ "lightColor", "lightmap", "resolution" });

    glUniform3fv(m_shader->GetUniformLocation("lightColor"), 1, glm::value_ptr(m_color));
    glUniform1i(m_shader->GetUniformLocation("lightmap"), 1);
}
