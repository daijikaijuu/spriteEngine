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
#include <GLFW/glfw3.h>
#include <vector>

namespace spriteEngine {
    class seGameLevel;
    class seSpriteTile;
}

using namespace spriteEngine;

class gameScene : public seScene {
public:
    gameScene(unsigned int width, unsigned int height);
    virtual ~gameScene();

    virtual void InitializeResources();

    void HandleInput(GLFWwindow *window, int key, int scancode, int action, int mods);
    void Update(GLfloat secondsElapsed);

private:
    seGameLevel *m_gameLevel;
    seSpriteTile *m_hero;
    GLboolean m_gravity;

    void MoveHero(GLfloat shiftX, GLfloat shiftY);
    void ScrollMap(GLfloat shiftX);
};

#endif /* gameScene_hpp */
