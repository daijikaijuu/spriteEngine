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

class gameHero;

class gameScene : public seScene {
public:
    gameScene(GLFWwindow *window, unsigned int width, unsigned int height);
    virtual ~gameScene();

    virtual void InitializeResources();

    void HandleInput();
    void Update(GLfloat secondsElapsed);

private:
    GLFWwindow *m_window;
    seGameLevel *m_gameLevel;
    gameHero *m_hero;
    seGenericSceneObject *m_backgroundMountain;
    GLfloat m_backgroundShift;
    GLboolean m_gravity;

    void MoveHero(GLfloat secondsElapsed);
    void ScrollMap(GLfloat shiftX);
};

#endif /* gameScene_hpp */
