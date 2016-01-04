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

    m_texture = new Texture(5);
    m_texture = TextureManager::GetInstance()->GetTexture("Data/Textures/campfire.png", 5);
    m_texture->setFiltering();

    ShapeData<TexturedVertex> *vertexData = shapeGenerator::generateTexturedQuad(0, -size / 2, m_z, size, size);

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(vertexData->vertices, vertexData->vertexBufferSize());
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    m_shader->Bind();
    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
    m_shader->RegisterUniform({ "projectionMatrix", "modelview", "texture", "spriteCount", "texShift" });

    m_VAO->Generate<TexturedVertex>(m_shader, vertexData, "inPosition", 0);
    m_VAO->Generate<TexturedVertex>(m_shader, vertexData, "inCoord", 1);

    GLint projection = m_shader->GetUniformLocation("projectionMatrix");
    if (projection != -1)
    {
        glm::mat4 p = glm::ortho(0.0f, 1.0f * 800, 1.0f * 600, 0.0f);
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
    }
    GLuint samplerLoc = m_shader->GetUniformLocation("texture");
    glUniform1i(samplerLoc, 0);
    samplerLoc = m_shader->GetUniformLocation("spriteCount");
    glUniform1i(samplerLoc, m_texture->GetItems());
    m_modelview = m_shader->GetUniformLocation("modelview");
    m_item = m_shader->GetUniformLocation("texShift");

    Move(0, 0);

    m_shader->UnBind();

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
