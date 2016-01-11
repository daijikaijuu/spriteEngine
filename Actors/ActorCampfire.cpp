#include "../homework02.h"
#include "../Render/Texture.h"
#include "../Render/Vertex.h"
#include "../Render/TextureManager.h"
#include "ActorCampfire.h"
#include <time.h>

ActorCampfire::ActorCampfire(GLfloat x, GLfloat y, GLfloat z, GLfloat size) :
    TexturedActor(x, y, size, z),
    m_item(0),
    m_animationItem(0)
{
    m_shader->Load("Data/Shaders/campfire");

    m_texture = TextureManager::GetInstance()->GetTexture("Data/Textures/campfire.png", 5);
    m_texture->setFiltering();

    TexturedShape *vertexData = shapeGenerator::generateTexturedQuad(0, -size / 2, m_z, size, size);
    InitializeVBO<TexturedShape>(vertexData);

    BindShaderAttributesAndUniforms();

    SetAttribute<TexturedShape>(m_shader, vertexData, "inPosition", 0);
    SetAttribute<TexturedShape>(m_shader, vertexData, "inCoord", 1);

    delete vertexData;
}

ActorCampfire::~ActorCampfire()
{
}

void ActorCampfire::Draw()
{
    TexturedActor::Draw();

    m_shader->Bind();
    glDrawArrays(GL_QUADS, 0, 4);
    m_shader->UnBind();
}

void ActorCampfire::Animate(GLint elapsedTime)
{
    GenericActor::Animate(elapsedTime);

    m_animationItem++;
    if (m_animationItem > (GLuint)m_texture->GetItems())
    {
        m_animationItem = 0;
    }

    m_shader->Bind();

    GLfloat x = 1.0f / 5.0f * m_animationItem;

    glUniform1f(m_item, x);
}

void ActorCampfire::BindShaderAttributesAndUniforms()
{
    TexturedActor::BindShaderAttributesAndUniforms();

    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
    m_shader->RegisterUniform({ "spriteCount", "texShift" });

    glUniform1i(m_shader->GetUniformLocation("spriteCount"), m_texture->GetItems());
    m_item = m_shader->GetUniformLocation("texShift");
}
