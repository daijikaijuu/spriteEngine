//
//  seEnums.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 28.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seEnums_h
#define seEnums_h

namespace spriteEngine {
    enum seResourceType {
        seRESOURCE_SHADER,
        seRESOURCE_SHADER_PROGRAM,
        seRESOURCE_TEXTURE,
        seRESOURCE_VBO,
        seRESOURCE_VAO,
    };

    enum seCollisionDirection {
        seCOLLISION_UP,
        seCOLLISION_DOWN,
        seCOLLISION_LEFT,
        seCOLLISION_RIGHT,
    };
}

#endif /* seEnums_h */
