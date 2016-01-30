//
//  seSprite.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seSprite_hpp
#define seSprite_hpp

#include "seSceneObject.hpp"

namespace spriteEngine {
    class seTexture;

    class seSprite : public seSceneObject {
    public:
        seSprite(bool centred, seTexture *texture, seProgram *shaderProgram = nullptr);
        virtual ~seSprite();

        void SetTexture(seTexture *texture);

        seTexture* Texture() { return m_texture; }

        virtual void Render();

    protected:
        virtual void Bind();
        virtual void Unbind();

    private:
        seTexture *m_texture;
    };
}

#endif /* seSprite_hpp */
