//
//  seSpriteTile.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 31.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seSpriteTile_hpp
#define seSpriteTile_hpp

#include "seSprite.hpp"
#include <stdio.h>

namespace spriteEngine {
    class seTexture;
    class seProgram;

    class seSpriteTile : public seSprite {
    public:
        seSpriteTile(bool centered, seProgram *shaderProgram, seTexture *texture, unsigned int itemsX, unsigned int itemCount);
        virtual ~seSpriteTile();

        virtual void Render();

        virtual void SetCurrentSprite(unsigned int currentSprite);
        virtual void SetMirrored(GLboolean mirrored);
        virtual void Animate();

    protected:
        unsigned int m_itemsX, m_itemCount;
        unsigned int m_currentSprite;
    };
}

#endif /* seSpriteTile_hpp */
