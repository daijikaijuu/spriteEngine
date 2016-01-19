#include "GenericActor.h"

GenericActor::GenericActor(GLfloat x, GLfloat y, GLfloat size, GLfloat z) :
    Logger(),
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
    DEBUG_OK();

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

bool GenericActor::DEBUG_OK() const
{
    HW_ASSERT(m_sceneWidth > 0);
    HW_ASSERT(m_sceneHeight > 0);
    HW_ASSERT(m_VAO->DEBUG_OK());
    HW_ASSERT(m_shader->DEBUG_OK());

    return true;
}

void GenericActor::UpdateMVP()
{
    if (m_MVP != -1)
    {
        m_shader->Bind();
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, 0.0f));
        glm::mat4 MVPmatrix = m_projection * m_view * m_model;
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

void GenericActor::InitializeShader(const string &vertexFilename, const string &fragmentFilename)
{
    m_shader->Load(vertexFilename, fragmentFilename);
}

string GenericActor::DEBUG_DUMP() const
{
    std::stringstream result;
    result << endl << " CLASS: " << GenericActor::class_type() << endl;
    result << DUMP_VAR(m_x) << DUMP_VAR(m_y) << DUMP_VAR(m_z) << endl;
    result << DUMP_VAR(m_sceneWidth) << DUMP_VAR(m_sceneHeight) << endl;
    result << DUMP_VAR(m_size) << endl;
    result << DUMP_VAR(m_elapsedTime) << endl;
    result << DUMP_VAR(m_MVP) << endl;
    result << "  m_shader:" << m_shader->DEBUG_DUMP();
    result << "  m_VAO:" << m_VAO->DEBUG_DUMP();
    return result.str();
}

TexturedActor::TexturedActor(GLfloat x, GLfloat y, GLfloat size, GLfloat z) :
    GenericActor(x, y, size, z),
    m_texture(NULL)
{
}

TexturedActor::~TexturedActor()
{
}

void TexturedActor::Draw()
{
    GenericActor::Draw();
    m_texture->BindTexture();
}

bool TexturedActor::DEBUG_OK() const
{
    HW_ASSERT(GenericActor::DEBUG_OK());
    HW_ASSERT(m_texture->DEBUG_OK());

    return true;
}

string TexturedActor::DEBUG_DUMP() const
{
    std::stringstream result;
    result << GenericActor::DEBUG_DUMP();
    result << " CLASS: " << TexturedActor::class_type() << endl;
    result << "  m_texture:" << m_texture->DEBUG_DUMP();
    return result.str();
}

void TexturedActor::BindShaderAttributesAndUniforms()
{
    GenericActor::BindShaderAttributesAndUniforms();

    m_shader->RegisterUniform({ "gSampler", "alpha" });

    glUniform1i(m_shader->GetUniformLocation("gSampler"), 0);
    glUniform1f(m_shader->GetUniformLocation("alpha"), 0.0f);
}
