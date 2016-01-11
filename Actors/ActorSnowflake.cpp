#include "../Render/Texture.h"
#include "../Render/TextureManager.h"
#include "../Render/Vertex.h"
#include "ActorSnowflake.h"

ActorSnowflake::ActorSnowflake(GLfloat x, GLfloat y, GLfloat size, GLuint sceneWidth, GLuint sceneHeight, GLfloat z) :
    TexturedActor(x, y, size, z),
    m_sceneWidth(sceneWidth),
    m_sceneHeight(sceneHeight),
    m_speed(0),
    m_deltaX(0),
    m_angle(0)
{
    m_shader->Load("Data/Shaders/basic", "Data/Shaders/snowflake");
    m_texture = TextureManager::GetInstance()->GetTexture("Data/Textures/snowflake.png");

    TexturedShape *vertexData = shapeGenerator::generateTexturedQuad(0, 0, m_z, m_size, m_size);
    InitializeVBO<TexturedShape>(vertexData);

    BindShaderAttributesAndUniforms();

    SetAttribute<TexturedShape>(vertexData, "inPosition", 0);
    SetAttribute<TexturedShape>(vertexData, "inCoord", 1);

    m_speed = (GLfloat)rand() / (RAND_MAX + 1);
    m_deltaX = (GLfloat)rand() / (RAND_MAX + 1);
    if (rand() % 2 > 1)
        m_deltaX *= -1;

    delete vertexData;
}

ActorSnowflake::~ActorSnowflake()
{
}

void ActorSnowflake::Draw()
{
    TexturedActor::Draw();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_shader->Bind();
    glDrawArrays(GL_QUADS, 0, 4);
    m_shader->UnBind();
    glDisable(GL_BLEND);
}

void ActorSnowflake::Animate(GLint elapsedTime)
{
    GenericActor::Animate(elapsedTime);

    Move(m_deltaX, m_speed * 8);
    if (m_y > m_sceneHeight + m_size)
    {
        m_x = GLfloat(rand() % m_sceneWidth);
        m_y = GLfloat(rand() % 50);
        m_speed = (GLfloat)rand() / (RAND_MAX + 1);
        m_deltaX = (GLfloat)rand() / (RAND_MAX + 1);
        if (rand() % 2 > 1)
            m_deltaX *= -1;
    }
    if ((m_x + m_size / 2 > m_sceneWidth) || (m_x - m_size / 2 < 0))
        m_deltaX *= -1;

    m_model = glm::rotate_slow(m_model, m_deltaX / 10, glm::vec3(0, 0, 1));
}

void ActorSnowflake::BindShaderAttributesAndUniforms()
{
    TexturedActor::BindShaderAttributesAndUniforms();

    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
}
