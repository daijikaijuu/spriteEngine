//
//  gameLevel.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 31.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef gameLevel_hpp
#define gameLevel_hpp

#include <stdio.h>
#include "seSceneObject.hpp"

namespace spriteEngine {
    class seProgram;
    class seTexture;

    class seGameLevel : public seGenericSceneObject {
    public:
        seGameLevel(seProgram *shaderProgram, seTexture *tileSet);
        virtual ~seGameLevel();

        virtual void Render();

    private:
        seTexture *m_tileSet;

        unsigned int m_width, m_height;
    };

}

#endif /* gameLevel_hpp */
