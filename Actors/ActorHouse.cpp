#include "../Render/Texture.h"
#include "../Render/TextureManager.h"
#include "../helpers.h"
#include "ActorHouse.h"

ActorHouse::ActorHouse(GLfloat x, GLfloat y, GLfloat z, GLfloat size) :
    GenericActor(x, y, z),
    m_size(size),
    m_modelview(0),
    m_texBrick(NULL)
{
    m_shader->Load("Data/Shaders/house");
    m_texBrick = TextureManager::get_instance()->GetTexture("Data/Textures/house.png");

    GLfloat Size = m_size / 2;
    TexturedVertex vertexData[8] = {
        glm::vec3(-Size, -m_size, m_z), glm::vec2(0, 1),
        glm::vec3(-Size, 0, m_z),       glm::vec2(0, 0),
        glm::vec3(Size, 0, m_z),        glm::vec2(1, 0),
        glm::vec3(Size, -m_size, m_z),  glm::vec2(1, 1),
    };

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(vertexData, sizeof(vertexData));
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    m_shader->Bind();
    m_shader->RegisterAttribute("inPosition");
    m_shader->RegisterAttribute("inCoord");
    m_shader->RegisterUniform("projectionMatrix");
    m_shader->RegisterUniform("modelview");
    m_shader->RegisterUniform("gSampler");

    m_VAO->Generate(m_shader->GetAttributeLocation("inPosition"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    m_VAO->Generate(m_shader->GetAttributeLocation("inCoord"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

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
}

ActorHouse::~ActorHouse()
{
    if (m_texBrick)
    {
        delete m_texBrick;
        m_texBrick = NULL;
    }
}

void ActorHouse::Draw()
{
    GenericActor::Draw();

    m_shader->Bind();
    m_texBrick->BindTexture();
    glDrawArrays(GL_QUADS, 0, 4);
    m_shader->UnBind();
}

void ActorHouse::Move(GLfloat xShift, GLfloat yShift)
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
