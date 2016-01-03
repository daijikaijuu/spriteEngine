#include "../Render/Texture.h"
#include "../Render/TextureManager.h"
#include "../Render/Vertex.h"
#include "ActorSnowflake.h"

ActorSnowflake::ActorSnowflake(GLfloat x, GLfloat y, GLfloat size, GLuint sceneWidth, GLuint sceneHeight, GLfloat z) :
    GenericActor(x, y, z),
    m_texture(NULL),
    m_modelview(0),
    m_size(size),
    m_sceneWidth(sceneWidth),
    m_sceneHeight(sceneHeight),
    m_speed(0),
    m_deltaX(0),
    m_angle(0)
{
    m_shader->Load("Data/Shaders/snowflake");
    m_texture = TextureManager::get_instance()->GetTexture("Data/Textures/snowflake.png");

    ShapeData<TexturedVertex> *vertexData = shapeGenerator::generateTexturedQuad(0, 0, m_z, m_size, m_size);

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(vertexData->vertices, vertexData->vertexBufferSize());
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    m_shader->Bind();
    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
    m_shader->RegisterUniform({ "projectionMatrix", "modelview", "gSampler" });

    m_VAO->Generate(m_shader->GetAttributeLocation("inPosition"), 3, GL_FLOAT, GL_FALSE, vertexData->itemSize(), vertexData->position(0));
    m_VAO->Generate(m_shader->GetAttributeLocation("inCoord"), 3, GL_FLOAT, GL_FALSE, vertexData->itemSize(), vertexData->position(1));

    GLint projection = m_shader->GetUniformLocation("projectionMatrix");
    if (projection != -1)
    {
        glm::mat4 p = glm::ortho(0.0f, 1.0f * 800, 1.0f * 600, 0.0f);
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
    }
    m_modelview = m_shader->GetUniformLocation("modelview");
    GLuint samplerLoc = m_shader->GetUniformLocation("gSampler");
    glUniform1i(samplerLoc, 0);

    Move(0, 0);

    m_shader->UnBind();

    m_speed = (GLfloat)rand() / (RAND_MAX + 1);
    m_deltaX = (GLfloat)rand() / (RAND_MAX + 1);
    if (rand() % 2 > 1)
        m_deltaX *= -1;

    delete vertexData;
}

ActorSnowflake::~ActorSnowflake()
{
    if (m_texture)
    {
        delete m_texture;
        m_texture = NULL;
    }
}

void ActorSnowflake::Draw()
{
    GenericActor::Draw();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_shader->Bind();
    m_texture->BindTexture();
    glDrawArrays(GL_QUADS, 0, 4);
    m_shader->UnBind();
    glDisable(GL_BLEND);
}

void ActorSnowflake::Animate()
{
    GenericActor::Animate();

    Move(0.0f + m_deltaX / 10, 0.0001f + m_speed / 5);
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
}

void ActorSnowflake::Move(GLfloat xShift, GLfloat yShift)
{
    GenericActor::Move(xShift, yShift);

    m_shader->Bind();
    if (m_modelview != -1)
    {
        m_angle += m_deltaX / 1000;
        glm::mat4 p = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, 0.0f));
        p = glm::rotate_slow(p, m_angle, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(m_modelview, 1, GL_FALSE, glm::value_ptr(p));
    }
    m_shader->UnBind();
}
