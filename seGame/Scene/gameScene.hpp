//
//  gameScene.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 31.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef gameScene_hpp
#define gameScene_hpp

#include "Render/seScene.hpp"
#include <stdio.h>

using namespace spriteEngine;

class gameScene : public seScene {
public:
    gameScene(unsigned int width, unsigned int height);
    virtual ~gameScene();

    virtual void InitializeResources();
};

#endif /* gameScene_hpp */
