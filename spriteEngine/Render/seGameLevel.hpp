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
#include "../Resources/seResource.hpp"
#include "../Utils/seCollisionRect.hpp"
#include "../Utils/seEnums.hpp"
#include <vector>

namespace tinyxml2 {
    class XMLElement;
}

namespace spriteEngine {
    class seProgram;
    class seTexture;

    struct seTile {
        GLuint id;
        GLuint mapX, mapY;
        seCollisionRect rect;
        GLboolean collidable;
        GLboolean onScreen;
        GLuint indexShift;

        seTile(GLuint ID, GLuint MapX, GLuint MapY, seCollisionRect Rect, GLboolean Collidable, GLboolean OnScreen, GLuint IndexShift) :
            id(ID),
            mapX(MapX), mapY(MapY),
            rect(Rect),
            collidable(Collidable),
            onScreen(OnScreen),
            indexShift(IndexShift)
        {
        }
    };

    struct seTileLayer {
        GLuint width;
        GLuint height;

        std::vector<seTile *> tiles;

        seTileLayer(GLuint Width, GLuint Height) :
            width(Width), height(Height),
            tiles()
        {
        }

        ~seTileLayer() {
            for (auto it : tiles) {
                delete it;
            }
            tiles.clear();
        }
    };

    class seGameLevel : public seGenericSceneObject, public seResource
    {
    public:
        seGameLevel(seProgram *shaderProgram, const std::string &resourceName);
        virtual ~seGameLevel();

        virtual GLfloat Width() const { return m_width * m_tileSize; }
        virtual GLfloat Height() const { return m_height * m_tileSize; }

        virtual void Render();
        virtual void UpdateMVP();

        GLboolean Collision(seCollisionRect rect, seCollisionDirection direction) const;

    private:
        seTexture *m_tileSet;
        GLfloat m_tileSize;
        GLuint m_indexBuffer;

        std::map<const std::string, seTileLayer *> m_layers;

        void ParseLayer(GLuint *index, GLuint tilesetColumns, GLuint tilesetRows, tinyxml2::XMLElement *element,
                        std::map<GLuint, seCollisionRect *> &tileBounds);
    };
}

#endif /* gameLevel_hpp */
