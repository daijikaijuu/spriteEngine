#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

VertexArrayObject::VertexArrayObject() :
    Logger(),
    m_VAO(0),
    m_VBO(NULL)
{
    m_VBO = new VertexBufferObject();

    glGenVertexArrays(1, &m_VAO);
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &m_VAO);

    if (m_VBO)
    {
        delete m_VBO;
        m_VBO = NULL;
    }
}

void VertexArrayObject::Generate(GLuint attribute, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer)
{
    Bind();
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(attribute, size, type, normalized, stride, pointer);
}

void VertexArrayObject::Bind() const
{
    if (!DEBUG_OK())
        CriticalError();

    glBindVertexArray(m_VAO);
}

void VertexArrayObject::UnBind() const
{
    glBindVertexArray(0);
}

GLuint VertexArrayObject::GetID() const
{
    return m_VAO;
}

VertexBufferObject* VertexArrayObject::GetVBO() const
{
    return m_VBO;
}

bool VertexArrayObject::DEBUG_OK() const
{
    if (!m_VBO->DEBUG_OK())
        CriticalError();

    HW_ASSERT(m_VAO > 0);

    return true;
}

string VertexArrayObject::DEBUG_DUMP() const
{
    std::stringstream result;
    result << endl;
    result << DUMP_VAR(m_VAO) << endl;
    result << "  m_VBO:" << m_VBO->DEBUG_DUMP();
    return result.str();
}
