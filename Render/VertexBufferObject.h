#pragma once

#include "..\homework02.h"

class VertexBufferObject
{
public:
    VertexBufferObject(GLuint size = 0);
    virtual ~VertexBufferObject();

    void AddData(GLvoid *ptrData, GLuint dataSize);
    void UploadDataToGPU(GLenum drawingHint);
    GLvoid* MapBufferToMemory(GLenum usageHint);
    GLvoid* MapSubBufferToMemory(GLenum usageHint, GLuint offset, GLuint lenght);
    void UnmapBuffer();

    void Bind(GLenum bufferType);

    GLuint GetID() const;
    GLvoid* GetDataPointer() const;

private:
    GLuint m_buffer;
    GLuint m_size;
    GLenum m_bufferType;
    std::vector<GLbyte> m_data;

    bool m_dataUploaded;
};


