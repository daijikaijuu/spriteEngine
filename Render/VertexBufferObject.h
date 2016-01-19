#pragma once

#include "../homework02.h"
#include "../Logger.h"

class VertexBufferObject : public Logger
{
public:
    VertexBufferObject(GLuint size = 0);
    virtual ~VertexBufferObject();

    void AddData(GLvoid *ptrData, GLsizeiptr dataSize);
    void UploadDataToGPU(GLenum drawingHint);
    GLvoid* MapBufferToMemory(GLenum usageHint);
    GLvoid* MapSubBufferToMemory(GLenum usageHint, GLuint offset, GLuint lenght);
    void UnmapBuffer();

    void Bind(GLenum bufferType);

    GLuint GetID() const;
    GLvoid* GetDataPointer() const;

    VertexBufferObject& operator=(const VertexBufferObject &vbo) = delete;
    VertexBufferObject(const VertexBufferObject &vbo) = delete;

    virtual string class_type() const { return "VertexBufferObject"; }
    virtual bool DEBUG_OK() const;
    virtual string DEBUG_DUMP() const;

private:
    GLuint m_buffer;
    GLuint m_size;
    GLenum m_bufferType;
    std::vector<GLbyte> m_data;

    bool m_dataUploaded;
};


