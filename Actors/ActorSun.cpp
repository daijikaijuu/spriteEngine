#include "../homework02.h"
#include "../helpers.h"
#include "../Render/Vertex.h"
#include "../Render/Shader.h"
#include "../Render/Texture.h"
#include "../Render/TextureManager.h"
#include "ActorSun.h"

ActorSun::ActorSun(GLfloat x, GLfloat y, GLfloat z, GLfloat size) :
    TexturedActor(x, y, size, z)
{
    InitializeShader("Data/Shaders/basic", "Data/Shaders/sun");
    m_texture = TextureManager::GetInstance()->GetTexture("Data/Textures/light.png");

    TexturedShape *vertexData = shapeGenerator::generateTexturedQuad(0, 0, m_z, m_size, m_size);
    InitializeVBO<TexturedShape>(vertexData);

    BindShaderAttributesAndUniforms();

    SetAttribute<TexturedShape>(vertexData, "inPosition", 0);
    SetAttribute<TexturedShape>(vertexData, "inCoord", 1);

    delete vertexData;
}

ActorSun::~ActorSun()
{
}

void ActorSun::Draw()
{
    TexturedActor::Draw();

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_ALPHA, GL_ONE); 
    m_shader->Bind();
    glDrawArrays(GL_QUADS, 0, 4);
    m_shader->UnBind();
    glDisable(GL_BLEND);
}

void ActorSun::Animate(GLint elapsedTime)
{
    TexturedActor::Animate(elapsedTime);
}

void ActorSun::BindShaderAttributesAndUniforms()
{
    TexturedActor::BindShaderAttributesAndUniforms();

    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
}
