//
//  gameScene.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 31.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "gameScene.hpp"
#include "gameHero.hpp"
#include "Render/seSprite.hpp"
#include "Render/seGameLevel.hpp"
#include "Resources/seProgram.hpp"
#include "Resources/seTexture.hpp"
#include "Resources/seResourceManager.hpp"
#include "Utils/seCollisionRect.hpp"

gameScene::gameScene(GLFWwindow *window, unsigned int width, unsigned height) :
    seScene((float)width, (float)height),
    m_window(window),
    m_gameLevel(nullptr),
    m_hero(nullptr),
    m_gravity(true),
    m_backgroundMountain(nullptr),
    m_backgroundShift(0)
{
    seAssert(m_window != nullptr);

    InitializeResources();

    seResourceManager *manager = seResourceManager::GetInstance();
    seGenericSceneObject *obj = nullptr;

    obj = new seSprite(false,
                       seNewShaderProgram("basic.vs", "basic.fs"),
                       manager->GetTexture("sky_01.png"));
    AddItem("sky", obj);
    obj->SetSize(0, 0, -1.0f, m_width, m_height);

    m_backgroundMountain = new seSprite(false,
                                        seNewShaderProgram("basic.vs", "basic.fs"),
                                        manager->GetTexture("bg2.png"));
    m_backgroundMountain->GetProgram()->SetUniform("alpha", 0.5f);
    m_backgroundMountain->SetSize(0, 150, -0.8f, m_width, m_height - 150);
    m_backgroundMountain->GetTexture()->SetFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
    AddItem("sceneObject:mountains", m_backgroundMountain);

    obj = new seSprite(true,
                       seNewShaderProgram("basic.vs", "basic.fs"),
                       manager->GetTexture("sun_01.png"));
    obj->GetProgram()->SetUniform("alpha", 0.5f);
    AddItem("sceneObject:sun", obj);
    obj->SetSize(600, 50, -0.9f, 200, 200);

    AddItem("sceneObject:iceman", m_hero = new gameHero());

    seSpriteTile *bird = new seSpriteTile(true,
                                          seNewShaderProgram("spriteTile.vs", "basic.fs"),
                                          seRManager->GetTexture("bird.png"), 14, 14);
    bird->GetProgram()->SetUniform("alpha", 0.2f);
    AddItem("sceneObject:bird", bird);
    bird->SetSize(50, 100, -0.8f, 50, 50);
    bird->AddItem(new seAnimation("animation:bird:movement",
                                  inc_fill_vector((uint32_t)14),
                                  0.1f),
                  true);

    m_gameLevel = new seGameLevel(seNewShaderProgram("basic.vs", "basic.fs"),
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
    seRManager->AddTexture("bg2.png");
    seRManager->AddTexture("sky_01.png");
    seRManager->AddTexture("sun_01.png");
    seRManager->AddTexture("bird.png");
}

void gameScene::HandleInput() {
    if ((glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) || (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)) {
        m_hero->SetMirrored(true);
        m_hero->IncHorizontalSpeed(-1.0f);
    }
    if ((glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) || (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)) {
        m_hero->SetMirrored(false);
        m_hero->IncHorizontalSpeed(1.0f);
    }
    if (((glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) ||
         (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) ||
         (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)) &&
        !m_hero->inJump())
    {
        m_hero->Jump();
    }
    if (((glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) ||
         (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)) &&
         !m_gravity)
    {
        m_hero->IncVerticalSpeed(5.0f);
    }

    if (glfwGetKey(m_window, GLFW_KEY_G) == GLFW_PRESS)
        m_gravity = !m_gravity;

    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, GL_TRUE);
}

void gameScene::Update(GLfloat secondsElapsed) {
    HandleInput();

    m_hero->UpdateStats();
    m_hero->DoAnimation(secondsElapsed);

    seGenericSceneObject *sun = GetItem("sceneObject:sun");
    sun->Rotate(secondsElapsed);

    seSpriteTile *bird = dynamic_cast<seSpriteTile *>(GetItem("sceneObject:bird"));
    GLfloat x = bird->X();
    if (x > (800 - bird->Width() / 2) && !bird->IsMirrored())
        bird->SetMirrored(true);
    if (x < bird->Width() / 2 && bird->IsMirrored())
        bird->SetMirrored(false);
    GLfloat dx = bird->IsMirrored() ? -1 : 1;
    bird->Move(dx, 0);
    bird->DoAnimation(secondsElapsed);
    bird->GetProgram()->Unbind();

    MoveHero(secondsElapsed);
}

void gameScene::MoveHero(GLfloat secondsElapsed) {
    GLfloat shiftX = m_hero->Speed().x;
    GLfloat shiftY = m_hero->Speed().y;
    seCollisionRect heroRect = m_hero->CollisionRect();

    if (heroRect.x <= 0.0f && m_gameLevel->X() >= 0.0f && shiftX < 0.0f)
        shiftX = 0;
    if (heroRect.Right() > m_width && m_gameLevel->X() + m_gameLevel->Width() <= m_width && shiftX > 0.0f)
        shiftX = 0;

    seCollisionRect rect = heroRect.Shift(shiftX, 0);
    if (m_gameLevel->Collision(rect, (shiftX > 0 ? seCOLLISION_RIGHT : seCOLLISION_LEFT)))
        shiftX = 0;
    rect = heroRect.Shift(0, shiftY);
    if (m_gameLevel->Collision(rect, (shiftY > 0 ? seCOLLISION_DOWN : seCOLLISION_UP)) || rect.y <= 0)
    {
        if (shiftY > 0)
            m_hero->Jump(false);
        shiftY = 0;
    }
    if (rect.Bottom() >= m_height) {
        Logger << "You loose. TODO: make something" << eol;
        if (shiftY > 0)
            m_hero->Jump(false);
        shiftY = 0;
    }

    if ((heroRect.x < m_width / 2) && (shiftX < 0.0f) && (m_gameLevel->X() < 0.0f)) {
        ScrollMap(-shiftX);
        m_hero->Move(0, shiftY);
    }
    else if ((heroRect.Right() > m_width / 2) && (shiftX > 0.0f) && (m_gameLevel->X() + m_gameLevel->Width() > m_width)) {
        ScrollMap(-shiftX);
        m_hero->Move(0, shiftY);
    }
    else
        m_hero->Move(shiftX, shiftY);

    if (shiftX != 0.0f) {
        float n = shiftX > 0.0f ? -1.0f : +1.0f;
        shiftX += secondsElapsed * 20.0f * n;
        if ((n < 0.0f && shiftX < 0.0f) || (n > 0.0f && shiftX > 0.0f))
            shiftX = 0.0f;
    }

    shiftY = m_gravity ? shiftY + secondsElapsed * 20.0f : 0.0f;

    m_hero->SetSpeed(glm::vec2(shiftX, shiftY));
}

void gameScene::ScrollMap(GLfloat shiftX) {
    m_gameLevel->Move(shiftX, 0);

    m_backgroundShift -= shiftX / 5000;
    m_backgroundMountain->GetProgram()->SetUniform("shiftX", m_backgroundShift);
    m_backgroundMountain->GetProgram()->Unbind();
}
