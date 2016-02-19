//
//  gameHero.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 18.02.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef gameHero_hpp
#define gameHero_hpp

#include "Render/seSpriteTile.hpp"
#include "gameSceneObjectMovement.hpp"

using namespace spriteEngine;

class gameHero : public seSpriteTile, public gameSceneObjectMovement {
public:
    gameHero();
    virtual ~gameHero();

    bool inJump() const { return m_isJumping; }
    void Jump(bool jumping = true);

protected:
    bool m_isJumping;
};

#endif /* gameHero_hpp */
