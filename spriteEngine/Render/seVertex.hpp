//
//  seVertex.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 28.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seVertex_h
#define seVertex_h

#include <glm/glm.hpp>

namespace spriteEngine {
    struct seVertexUV {
        glm::vec3 coord;
        glm::vec2 uv;
    };

    static seVertexUV seQUAD_Centred_UV[] = {
        glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 0.0f),
        glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f),
        glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
    };

    static seVertexUV seQUAD_UV[] = {
        glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f),
    };
}

#endif /* seVertex_h */
