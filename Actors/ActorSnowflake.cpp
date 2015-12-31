#include "../Render/Texture.h"
#include "../Render/TextureManager.h"
#include "../helpers.h"
#include "ActorSnowflake.h"

ActorSnowflake::ActorSnowflake(GLfloat x, GLfloat y, GLfloat size, GLuint sceneWidth, GLuint sceneHeight, GLfloat z) :
    GenericActor(x, y, z),
    m_texture(NULL),
    m_modelview(0),
    m_size(size),
    m_sceneWidth(sceneWidth),
    m_sceneHeight(sceneHeight),
    m_speed(0),
    m_deltaX(0)
{
    m_shader->Load("Data/Shaders/snowflake");
    m_texture = TextureManager::get_instance()->GetTexture("Data/Textures/snowflake.png");
    m_texture->setFiltering();

    GLfloat Size = m_size / 2;
    TexturedVertex quad[4] = {
        glm::vec3(-Size, -Size, m_z), glm::vec2(0, 0), glm::vec3(1, 1, 1),
        glm::vec3(-Size, Size, m_z),  glm::vec2(0, 1), glm::vec3(1, 1, 1),
        glm::vec3(Size, Size, m_z),   glm::vec2(1, 1), glm::vec3(1, 1, 1),
        glm::vec3(Size, -Size, m_z),  glm::vec2(1, 0), glm::vec3(1, 1, 1),
    };

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(quad, sizeof(quad));
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    m_shader->Bind();
    m_shader->RegisterAttribute("inPosition");
    m_shader->RegisterAttribute("inCoord");
    m_shader->RegisterUniform("projectionMatrix");
    m_shader->RegisterUniform("modelview");
    m_shader->RegisterUniform("gSampler");

    m_VAO->Generate(m_shader->GetAttributeLocation("inPosition"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    m_VAO->Generate(m_shader->GetAttributeLocation("inCoord"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

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
    if ((m_x + m_size / 2 > m_sceneWidth) || (m_x + m_size < 0))
        m_deltaX *= -1;
}

void ActorSnowflake::Move(GLfloat xShift, GLfloat yShift)
{
    GenericActor::Move(xShift, yShift);

    m_shader->Bind();
    if (m_modelview != -1)
    {
        glm::mat4 p = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, 0.0f));
        glUniformMatrix4fv(m_modelview, 1, GL_FALSE, glm::value_ptr(p));
    }
    m_shader->UnBind();
}
