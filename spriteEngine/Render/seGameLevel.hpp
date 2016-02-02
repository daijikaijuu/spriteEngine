//
//  gameLevel.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 31.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef gameLevel_hpp
#define gameLevel_hpp

#include "seSceneObject.hpp"
#include "../Utils/seCollisionRect.hpp"
#include <vector>

namespace spriteEngine {
    class seProgram;
    class seTexture;

    struct seTile {
        GLuint id;
        GLuint mapX, mapY;
        seCollisionRect rect;
        GLboolean collidable;
        GLboolean onScreen;

        seTile(GLuint ID, GLuint MapX, GLuint MapY, seCollisionRect Rect, GLboolean Collidable, GLboolean OnScreen) :
            id(ID),
            mapX(MapX), mapY(MapY),
            rect(Rect),
            collidable(Collidable),
            onScreen(OnScreen)
        {
        }
    };

    class seGameLevel : public seGenericSceneObject {
    public:
        seGameLevel(seProgram *shaderProgram, seTexture *tileSet);
        virtual ~seGameLevel();

        virtual void Render();
        virtual void UpdateMVP();

        std::vector<seTile> Tiles() const { return m_tiles; }
        GLboolean IsTileInCoordCollidable(GLfloat x, GLfloat y) const;

    private:
        seTexture *m_tileSet;
        GLfloat m_tileSize;
        GLuint m_indexBuffer;

        std::vector<seTile> m_tiles;
    };

}

#endif /* gameLevel_hpp */
