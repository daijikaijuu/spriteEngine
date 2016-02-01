//
//  seScene.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seScene_hpp
#define seScene_hpp

#include "../Utils/seCollection.hpp"
#include "seSceneObject.hpp"
#include <stdio.h>

namespace spriteEngine {
    class seScene : public seCollection<seGenericSceneObject> {
    public:
        seScene(GLfloat width, GLfloat height);
        virtual ~seScene();

        virtual void InitializeResources();
        virtual void Render();

    protected:
        GLfloat m_width, m_height;
    };
}

#endif /* seScene_hpp */
