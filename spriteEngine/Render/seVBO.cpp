//
//  seVBO.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 28.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seVBO.hpp"
#include "../Debug/seDebug.hpp"

namespace spriteEngine {
    seVBO::seVBO(GLuint size) :
        seResource(seResourceType::seRESOURCE_VBO, "VertexBufferObject"),
        m_size(size),
        m_bufferType(0),
        m_data(),
        m_dataUploaded(false)
    {
        glGenBuffers(1, &m_ID);
        m_data.reserve(m_size);
    }

    seVBO::~seVBO() {
        Unbind();

        glDeleteBuffers(1, &m_ID);
        m_dataUploaded = false;
        m_data.clear();
    }

    void seVBO::Bind(GLenum bufferType) {
        seAssert(m_ID);

        m_bufferType = bufferType;
        glBindBuffer(m_bufferType, m_ID);
    }

    void seVBO::Unbind() {
        glUnmapBuffer(m_bufferType);
    }

    void seVBO::AddData(GLvoid *ptrData, GLsizeiptr dataSize) {
        seAssert(dataSize > 0);
        
        m_data.insert(m_data.end(), (GLbyte *)ptrData, (GLbyte *)ptrData + dataSize);
    }

    void seVBO::UploadDataToGPU(GLenum drawingHint) {
        glBufferData(m_bufferType, m_data.size(), &m_data[0], drawingHint);
        m_dataUploaded = true;
        m_data.clear();
    }

    GLvoid* seVBO::MapBufferToMemory(GLenum usageHint) {
        if (!m_dataUploaded)
            return nullptr;
        GLvoid *ptrRes = glMapBuffer(m_bufferType, usageHint);
        return ptrRes;
    }

    GLvoid* seVBO::MapSubBufferToMemory(GLenum usageHint, GLuint offset, GLuint lenght) {
        if (!m_dataUploaded)
            return nullptr;
        GLvoid *ptrRes = glMapBufferRange(m_bufferType, offset, lenght, usageHint);
        return ptrRes;
    }

    GLvoid* seVBO::GetDataPointer() const {
        if (m_dataUploaded)
            return nullptr;
        return (GLvoid *)(intptr_t)m_data[0];
    }
}
