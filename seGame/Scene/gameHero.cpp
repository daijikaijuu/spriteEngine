//
//  gameHero.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 18.02.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "gameHero.hpp"
#include "Resources/Resources.hpp"

gameHero::gameHero() :
    seSpriteTile(false, seNewShaderProgram("spriteTile.vs", "basic.fs"), seRManager->AddTexture("iceman.png"), 5, 5),
    gameSceneObjectMovement()
{
    GetProgram()->SetUniform("alpha", 0.2f);
    SetSize(50, 310, 0.5f, 40, 69);
}

gameHero::~gameHero() {
}
