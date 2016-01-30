//
//  seVBO.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 28.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seVBO_hpp
#define seVBO_hpp

#include "../Resources/seResource.hpp"
#include <stdio.h>
#include <vector>

namespace spriteEngine {
    class seVBO : public seResource {
        GLuint m_size;
        GLenum m_bufferType;
        std::vector<GLbyte> m_data;
        bool m_dataUploaded;
    public:
        seVBO(GLuint size = 0);
        virtual ~seVBO();

        virtual void Bind(GLenum bufferType);
        virtual void Unbind();

        void AddData(GLvoid *ptrData, GLsizeiptr dataSize);
        void UploadDataToGPU(GLenum drawingHint);
        GLvoid* MapBufferToMemory(GLenum usageHint);
        GLvoid* MapSubBufferToMemory(GLenum usageHint, GLuint offset, GLuint lenght);
        GLvoid* GetDataPointer() const;
    };
}

#endif /* seVBO_hpp */
