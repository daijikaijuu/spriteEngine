#include "GenericActor.h"

GenericActor::GenericActor(GLfloat x, GLfloat y, GLfloat size, GLfloat z) :
    m_x(x),
    m_y(y),
    m_z(z),
    m_size(size),
    m_sceneWidth(0),
    m_sceneHeight(0),
    m_projection(-1),
    m_modelview(-1),
    m_shader(NULL),
    m_VAO(NULL),
    m_elapsedTime(0)
{
    m_VAO = new VertexArrayObject();
    m_shader = new Shader();
}

GenericActor::~GenericActor()
{
    if (m_shader)
    {
        delete m_shader;
        m_shader = NULL;
    }

    if (m_VAO)
    {
        delete m_VAO;
        m_VAO = NULL;
    }
}

void GenericActor::Draw()
{
    m_VAO->Bind();
}

void GenericActor::Animate(GLint elapsedTime)
{
    m_elapsedTime = elapsedTime;
}

void GenericActor::Move(GLfloat xShift, GLfloat yShift)
{
    m_x += xShift;
    m_y += yShift;

    UpdateModelView();
}

void GenericActor::MoveTo(GLfloat x, GLfloat y)
{
    m_x = x;
    m_y = y;

    UpdateModelView();
}

void GenericActor::ResizeScene(GLsizei width, GLsizei height)
{
    m_sceneWidth = width;
    m_sceneHeight = height;

    if (m_projection != -1)
    {
        m_shader->Bind();
        glm::mat4 p = glm::ortho(0.0f, 1.0f * width, 1.0f * height, 0.0f);
        glUniformMatrix4fv(m_projection, 1, GL_FALSE, glm::value_ptr(p));
        m_shader->UnBind();
    }
}

void GenericActor::UpdateModelView()
{
    if (m_modelview != -1)
    {
        m_shader->Bind();
        glm::mat4 p = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, 0.0f));
        glUniformMatrix4fv(m_modelview, 1, GL_FALSE, glm::value_ptr(p));
        m_shader->UnBind();
    }
}

TexturedActor::TexturedActor(GLfloat x, GLfloat y, GLfloat size, GLfloat z) :
    GenericActor(x, y, size, z),
    m_texture(NULL)
{
}

TexturedActor::~TexturedActor()
{
    if (m_texture)
    {
        delete m_texture;
        m_texture = NULL;
    }
}

void TexturedActor::Draw()
{
    GenericActor::Draw();
    m_texture->BindTexture();
}
