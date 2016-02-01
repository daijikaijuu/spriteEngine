//
//  seSprite.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seSprite.hpp"
#include "../Resources/seTexture.hpp"
#include "../Debug/Debug.hpp"

namespace spriteEngine {
    seSprite::seSprite(bool centered, seProgram *shaderProgram, seTexture *texture) :
        seSceneObject(centered, shaderProgram),
        m_texture(texture)
    {
        seAssert(texture);
        seAssert(texture->Type() == seResourceType::seRESOURCE_TEXTURE);
    }

    seSprite::~seSprite() {
    }

    void seSprite::SetTexture(seTexture *texture) {
        seAssert(texture);

        m_texture = texture;
    }

    void seSprite::Render() {
        seSceneObject::Render();

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        Unbind();
    }

    void seSprite::Bind() {
        seSceneObject::Bind();
        m_texture->Bind();
    }

    void seSprite::Unbind() {
        seSceneObject::Unbind();
        m_texture->seResource::Unbind();
    }
}
