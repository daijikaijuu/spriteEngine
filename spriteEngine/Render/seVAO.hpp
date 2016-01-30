//
//  seVAO.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 28.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seVAO_hpp
#define seVAO_hpp

#include "../Resources/seResource.hpp"
#include <stdio.h>

namespace spriteEngine {
    class seVAO : public seResource {
    public:
        seVAO();
        virtual ~seVAO();

        virtual void Bind();
        virtual void Unbind();

        void Generate(GLuint attribute, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer);
    };
}

#endif /* seVAO_hpp */
