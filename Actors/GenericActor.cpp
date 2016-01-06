#include "GenericActor.h"

GenericActor::GenericActor(GLfloat x, GLfloat y, GLfloat size, GLfloat z) :
    m_x(x),
    m_y(y),
    m_z(z),
    m_size(size),
    m_sceneWidth(0),
    m_sceneHeight(0),
    m_MVP(-1),
    m_model(glm::mat4(1.0f)),
    m_view(glm::mat4(1.0f)),
    m_projection(glm::mat4(1.0f)),
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

    UpdateMVP();
}

void GenericActor::MoveTo(GLfloat x, GLfloat y)
{
    m_x = x;
    m_y = y;

    UpdateMVP();
}

void GenericActor::ResizeScene(GLsizei width, GLsizei height)
{
    m_sceneWidth = width;
    m_sceneHeight = height;

    m_projection = glm::ortho(0.0f, 1.0f * width, 1.0f * height, 0.0f);

    UpdateMVP();
}

void GenericActor::Initialize()
{
    BindShaderAttributesAndUniforms();
}

void GenericActor::UpdateMVP()
{
    if (m_MVP != -1)
    {
        m_shader->Bind();
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, 0.0f));
        glm::mat4 MVPmatrix = m_projection * m_view;
        glUniformMatrix4fv(m_MVP, 1, GL_FALSE, glm::value_ptr(MVPmatrix));
        m_shader->UnBind();
    }
}

void GenericActor::BindShaderAttributesAndUniforms()
{
    m_shader->Bind();

    m_shader->RegisterUniform("MVP");
    m_MVP = m_shader->GetUniformLocation("MVP");
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

void TexturedActor::UpdateMVP()
{
    GenericActor::UpdateMVP();
}
