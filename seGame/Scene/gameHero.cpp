//
//  gameHero.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 18.02.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "gameHero.hpp"
#include "Resources/Resources.hpp"

const std::string ANIMATION_HERO_MOVEMENT = "animation:hero:movement";

gameHero::gameHero() :
    seSpriteTile(false, seNewShaderProgram("spriteTile.vs", "basic.fs"), seRManager->AddTexture("iceman.png"), 5, 5),
    gameSceneObjectMovement(),
    m_isJumping(false)
{
    GetProgram()->SetUniform("alpha", 0.2f);
    SetSize(50, 310, 0.5f, 40, 69);

    // Add animations
    AddItem(new seAnimation(ANIMATION_HERO_MOVEMENT,
                            { 0, 1, 2, 3, 4 },
                            0.05f));
}

gameHero::~gameHero() {
}

void gameHero::Jump(bool jumping) {
    m_isJumping = jumping;
    IncVerticalSpeed(-10.0f);
}

void gameHero::UpdateStats() {
    if (!IsMoving()) {
        if (CurrentAnimation())
            StopAnimation();
    } else {
        if (!CurrentAnimation() || CurrentAnimation()->Name() != ANIMATION_HERO_MOVEMENT)
            SetCurrentAnimation(ANIMATION_HERO_MOVEMENT);
    }
}
