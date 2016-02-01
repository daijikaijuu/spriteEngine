//
//  seSpriteTile.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 31.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seSprite.hpp"
#include "seSpriteTile.hpp"
#include "../Debug/Debug.hpp"
#include "../Resources/Resources.hpp"
#include <cmath>

namespace spriteEngine {
    seSpriteTile::seSpriteTile(bool centered, seProgram *shaderProgram, seTexture *texture, unsigned int itemsX, unsigned int itemCount) :
        seSprite(centered, shaderProgram, texture),
        m_itemsX(itemsX), m_itemCount(itemCount),
        m_currentSprite(0)
    {
        m_shaderProgram->SetUniform("spriteCountX", m_itemsX);
        GLuint itemsY = ceil((GLfloat)itemCount / itemsX);
        m_shaderProgram->SetUniform("spriteCountY", itemsY);
        SetCurrentSprite(m_currentSprite);

        m_texture->SetFiltering(GL_LINEAR, GL_REPEAT);
    }

    seSpriteTile::~seSpriteTile() {
        
    }

    void seSpriteTile::Render() {
        seSprite::Render();
    }

    void seSpriteTile::SetCurrentSprite(unsigned int currentSprite) {
        m_currentSprite = currentSprite;
        m_shaderProgram->SetUniform("spriteCurrent", m_currentSprite);
    }

    void seSpriteTile::SetMirrored(GLboolean mirrored) {
        m_mirrored = mirrored;
        m_shaderProgram->SetUniform("mirrorTexture", (int)mirrored);
    }

    void seSpriteTile::Animate() {
        SetCurrentSprite(++m_currentSprite);
        if (m_currentSprite >= m_itemCount - 1) m_currentSprite = 0;
    }
}
