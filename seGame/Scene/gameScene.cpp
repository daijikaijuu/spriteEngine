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
#include "Resources/seResourceManager.hpp"

gameScene::gameScene(unsigned int width, unsigned height) :
    seScene((float)width, (float)height)
{
    InitializeResources();

    seResourceManager *manager = seResourceManager::GetInstance();
    seGenericSceneObject *obj = nullptr;

    obj = new seSprite(false,
                       new seProgram(seRManager->GetShader("basic.vs"), seRManager->GetShader("basic.fs")),
                       manager->GetTexture("sky_01.png"));
    AddItem("sky", obj);
    obj->SetSize(0, 0, -1.0f, m_width, m_height);

    obj = new seSprite(false,
                       new seProgram(seRManager->GetShader("basic.vs"), seRManager->GetShader("basic.fs")),
                       manager->GetTexture("bg2.png"));
    obj->GetProgram()->SetUniform("alpha", 0.7f);
    AddItem("sceneObject:mountains", obj);
    obj->SetSize(0, 150, -0.8f, m_width, m_height - 150);

    obj = new seSprite(true,
                       new seProgram(seRManager->GetShader("basic.vs"), seRManager->GetShader("basic.fs")),
                       manager->GetTexture("sun_01.png"));
    obj->GetProgram()->SetUniform("alpha", 0.7f);
    AddItem("sceneObject:sun", obj);
    obj->SetSize(600, 50, -0.9f, 200, 200);

    obj = new seSpriteTile(false,
                           new seProgram(seRManager->GetShader("spriteTile.vs"), seRManager->GetShader("basic.fs")),
                           manager->GetTexture("iceman.png"), 5, 5);
    obj->GetProgram()->SetUniform("alpha", 0.7f);
    AddItem("sceneObject:iceman", obj);
    obj->SetSize(50, 600 - 95, 0, 52, 95);

    obj = new seSpriteTile(true,
                           new seProgram(seRManager->GetShader("spriteTile.vs"), seRManager->GetShader("basic.fs")),
                           seRManager->GetTexture("bird.png"), 14, 14);
    obj->GetProgram()->SetUniform("alpha", 0.7f);
    AddItem("sceneObject:bird", obj);
    obj->SetSize(50, 100, 0, 50, 50);

    obj = new seGameLevel(new seProgram(seRManager->GetShader("basic.vs"), seRManager->GetShader("basic.fs")),
                          seRManager->GetTexture("tileset.png"));
    obj->GetProgram()->SetUniform("alpha", 0.7f);
    AddItem("sceneObject:gameLevel", obj);
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
    seRManager->AddTexture("tileset.png");
    seRManager->AddTexture("bird.png");
}
