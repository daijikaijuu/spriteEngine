//
//  seTexture.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seTexture_hpp
#define seTexture_hpp

#include "seResource.hpp"
#include <stdio.h>
#include <string>

namespace spriteEngine {
    class seTexture : public seResource {
    public:
        seTexture(const std::string &resourceName, bool useEmptyTexture = false);
        virtual ~seTexture();

        virtual void Bind(GLuint textureUnit = 0);
        virtual void Unbind();

        void SetFiltering(GLint minMagFiler = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);

    protected:
        GLuint m_Sampler;

        virtual void LoadFromFile(bool fallbackToEmptyTexture);
    };
}

#endif /* seTexture_hpp */
