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
#include "seAnimation.hpp"

namespace spriteEngine {
    class seTexture;
    class seProgram;

    class seSpriteTile :
        public seSprite,
        public seAnimator
    {
    public:
        seSpriteTile(bool centered, seProgram *shaderProgram, seTexture *texture, uint32_t itemsX, uint32_t itemCount);
        virtual ~seSpriteTile();

        virtual void Render();

        virtual void SetCurrentSprite(uint32_t currentSprite);
        virtual void SetMirrored(bool mirrored);
        virtual void Animate();
        virtual void DoAnimation(float_t elapsedTime);

    protected:
        uint32_t m_itemsX, m_itemCount;
        uint32_t m_currentSprite;
    };
}

#endif /* seSpriteTile_hpp */
