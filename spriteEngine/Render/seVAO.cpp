//
//  seVAO.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 28.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seVAO.hpp"
#include "../Debug/seDebug.hpp"

namespace spriteEngine {
    seVAO::seVAO() :
        seResource(seResourceType::seRESOURCE_VAO, "VertexArrayBuffer")
    {
        glGenVertexArrays(1, &m_ID);
        Bind();
    }

    seVAO::~seVAO() {
        Unbind();
        glDeleteVertexArrays(1, &m_ID);
    }

    void seVAO::Bind() {
        seAssert(m_ID);

        glBindVertexArray(m_ID);
    }

    void seVAO::Unbind() {
        glBindVertexArray(0);
    }

    void seVAO::Generate(GLuint attribute, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer) {
        seAssert(size > 0);
        seAssert(stride > 0);
        
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, size, type, normalized, stride, pointer);
    }
}
