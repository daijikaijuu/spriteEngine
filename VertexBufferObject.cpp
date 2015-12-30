#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(GLuint size) :
    m_buffer(0),
    m_size(size),
    m_dataUploaded(false),
    m_bufferType(0),
    m_data()
{
    glGenBuffers(1, &m_buffer);
    m_data.reserve(m_size);
}

VertexBufferObject::~VertexBufferObject()
{
    glDeleteBuffers(1, &m_buffer);
    m_dataUploaded = false;
    m_data.clear();
}

void VertexBufferObject::AddData(GLvoid* ptrData, GLuint dataSize)
{
    m_data.insert(m_data.end(), (GLbyte*)ptrData, (GLbyte*)ptrData + dataSize);
}

void VertexBufferObject::UploadDataToGPU(GLenum drawingHint)
{
    glBufferData(m_bufferType, m_data.size(), &m_data[0], drawingHint);
    m_dataUploaded = true;
    m_data.clear();
}

GLvoid* VertexBufferObject::MapBufferToMemory(GLenum usageHint)
{
    if (!m_dataUploaded)
        return nullptr;
    GLvoid *ptrRes = glMapBuffer(m_bufferType, usageHint);
    return ptrRes;
}

GLvoid * VertexBufferObject::MapSubBufferToMemory(GLenum usageHint, GLuint offset, GLuint lenght)
{
    if (!m_dataUploaded)
        return nullptr;
    GLvoid *ptrRes = glMapBufferRange(m_bufferType, offset, lenght, usageHint);
    return ptrRes;
}

void VertexBufferObject::UnmapBuffer()
{
    glUnmapBuffer(m_bufferType);
}

void VertexBufferObject::Bind(GLenum bufferType)
{
    m_bufferType = bufferType;
    glBindBuffer(m_bufferType, m_buffer);
}

GLuint VertexBufferObject::GetID() const
{
    return m_buffer;
}

GLvoid* VertexBufferObject::GetDataPointer() const
{
    if (m_dataUploaded)
        return nullptr;
    return (GLvoid*)m_data[0];
}
