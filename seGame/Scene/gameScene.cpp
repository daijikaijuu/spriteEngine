//
//  gameScene.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 31.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "gameScene.hpp"
#include "Render/seSprite.hpp"
#include "Render/seSpriteTile.hpp"
#include "Render/seGameLevel.hpp"
#include "Resources/seProgram.hpp"
#include "Resources/seTexture.hpp"
#include "Resources/seResourceManager.hpp"
#include "Utils/seCollisionRect.hpp"

gameScene::gameScene(unsigned int width, unsigned height) :
    seScene((float)width, (float)height),
    m_gameLevel(nullptr),
    m_hero(nullptr),
    m_gravity(true),
    m_backgroundMountain(nullptr),
    m_backgroundShift(0)
{
    InitializeResources();

    seResourceManager *manager = seResourceManager::GetInstance();
    seGenericSceneObject *obj = nullptr;

    obj = new seSprite(false,
                       new seProgram(seRManager->GetShader("basic.vs"), seRManager->GetShader("basic.fs")),
                       manager->GetTexture("sky_01.png"));
    AddItem("sky", obj);
    obj->SetSize(0, 0, -1.0f, m_width, m_height);

    m_backgroundMountain = new seSprite(false,
                                        new seProgram(seRManager->GetShader("basic.vs"),
                                                      seRManager->GetShader("basic.fs")),
                                        manager->GetTexture("bg2.png"));
    m_backgroundMountain->GetProgram()->SetUniform("alpha", 0.5f);
    m_backgroundMountain->SetSize(0, 150, -0.8f, m_width, m_height - 150);
    m_backgroundMountain->GetTexture()->SetFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
    AddItem("sceneObject:mountains", m_backgroundMountain);

    obj = new seSprite(true,
                       new seProgram(seRManager->GetShader("basic.vs"), seRManager->GetShader("basic.fs")),
                       manager->GetTexture("sun_01.png"));
    obj->GetProgram()->SetUniform("alpha", 0.5f);
    AddItem("sceneObject:sun", obj);
    obj->SetSize(600, 50, -0.9f, 200, 200);

    m_hero = new seSpriteTile(false,
                              new seProgram(seRManager->GetShader("spriteTile.vs"), seRManager->GetShader("basic.fs")),
                              manager->GetTexture("iceman.png"), 5, 5);
    m_hero->GetProgram()->SetUniform("alpha", 0.2f);
    m_hero->SetSize(50, 310, 0.5f, 42, 75);
    AddItem("sceneObject:iceman", m_hero);

    obj = new seSpriteTile(true,
                           new seProgram(seRManager->GetShader("spriteTile.vs"), seRManager->GetShader("basic.fs")),
                           seRManager->GetTexture("bird.png"), 14, 14);
    obj->GetProgram()->SetUniform("alpha", 0.2f);
    AddItem("sceneObject:bird", obj);
    obj->SetSize(50, 100, -0.8f, 50, 50);

    m_gameLevel = new seGameLevel(new seProgram(seRManager->GetShader("basic.vs"), seRManager->GetShader("basic.fs")),
                                  "level01.tmx");
    m_gameLevel->GetProgram()->SetUniform("alpha", 0.5f);
    AddItem("sceneObject:gameLevel:" + m_gameLevel->Name(), m_gameLevel);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

gameScene::~gameScene() {
}

void gameScene::InitializeResources() {
    seScene::InitializeResources();

    // Load shaders
    seRManager->AddShader("basic.vs");
    seRManager->AddShader("basic.fs");
    seRManager->AddShader("spriteTile.vs");

    // Load textures
    seRManager->AddTexture("house.png");
    seRManager->AddTexture("campfire.png");
    seRManager->AddTexture("snowflake.png");
    seRManager->AddTexture("bg2.png");
    seRManager->AddTexture("sky_01.png");
    seRManager->AddTexture("sun_01.png");
    seRManager->AddTexture("iceman.png");
    seRManager->AddTexture("bird.png");
}

void gameScene::HandleInput(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_A) {
        ScrollMap(-10.0f);
    }
    if (key == GLFW_KEY_D) {
        ScrollMap(10.0f);
    }

    if (key == GLFW_KEY_G && action == GLFW_RELEASE)
        m_gravity = !m_gravity;

    static GLuint spr;
    if (key == GLFW_KEY_RIGHT) {
        spr++;
        m_hero->SetMirrored(false);
        MoveHero(5.0f, 0.0f);
    }
    if (key == GLFW_KEY_LEFT) {
        spr++;
        m_hero->SetMirrored(true);
        MoveHero(-5.0f, 0.0f);
    }
    if (key == GLFW_KEY_UP) {
        spr++;
        MoveHero(0.0f, -10.0f);
    }
    if (key == GLFW_KEY_DOWN) {
        spr++;
        MoveHero(0.0f, 10.0f);
    }

    m_hero->GetProgram()->SetUniform("spriteCurrent", spr);
    m_hero->GetProgram()->Unbind();
    if (spr > 5) spr = 1;
}

void gameScene::Update(GLfloat secondsElapsed) {
    seGenericSceneObject *sun = GetItem("sceneObject:sun");
    sun->Rotate(secondsElapsed);

    seGenericSceneObject *bird = GetItem("sceneObject:bird");
    GLfloat x = bird->X();
    if (x > (800 - bird->Width() / 2) && !bird->IsMirrored())
        bird->SetMirrored(true);
    if (x < bird->Width() / 2 && bird->IsMirrored())
        bird->SetMirrored(false);
    GLfloat dx = bird->IsMirrored() ? -1 : 1;
    bird->Move(dx, 0);
    bird->GetProgram()->Unbind();

    static GLfloat counter;
    counter += secondsElapsed;
    if (counter > 0.2f) {
        bird->Animate();
        bird->GetProgram()->Unbind();
        counter = 0;
    }

    if (m_gravity) {
        MoveHero(0, secondsElapsed * 200);
    }
}

void gameScene::MoveHero(GLfloat shiftX, GLfloat shiftY) {
    seCollisionRect heroRect = m_hero->CollisionRect();

    if (heroRect.x <= 0 && m_gameLevel->X() <= 0 && shiftX < 0)
        return;
    if (heroRect.Right() > m_width && m_gameLevel->X() + m_gameLevel->Width() <= m_width && shiftX > 0)
        return;

    seCollisionRect rect = heroRect.Shift(shiftX, 0);
    if (m_gameLevel->Collision(rect, (shiftX > 0 ? seCOLLISION_RIGHT : seCOLLISION_LEFT)))
        return;
    rect = heroRect.Shift(0, shiftY);
    if (m_gameLevel->Collision(rect, (shiftY > 0 ? seCOLLISION_DOWN : seCOLLISION_UP)) || rect.y <= 0)
        return;
    if (rect.Bottom() >= m_height) {
        Logger << "You loose. TODO: make something" << eol;
        return;
    }

    if ((heroRect.x < m_width / 2) &&
        (shiftX < 0) &&
        (m_gameLevel->X() < 0))
    {
        ScrollMap(-shiftX);
        return;
    }
    if ((heroRect.Right() > m_width / 2) &&
        (shiftX > 0) &&
        (m_gameLevel->X() + m_gameLevel->Width() > m_width))
    {
        ScrollMap(-shiftX);
        return;
    }

    m_hero->Move(shiftX, shiftY);
}

void gameScene::ScrollMap(GLfloat shiftX) {
    m_gameLevel->Move(shiftX, 0);

    m_backgroundShift -= shiftX / 5000;
    m_backgroundMountain->GetProgram()->SetUniform("shiftX", m_backgroundShift);
    m_backgroundMountain->GetProgram()->Unbind();
}
