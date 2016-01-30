//
//  seScene.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seScene.hpp"
#include "seSprite.hpp"
#include "../Resources/seResourceManager.hpp"

namespace spriteEngine {
    seScene::seScene(GLfloat width, GLfloat height) :
        m_width(width), m_height(height)
    {
        InitializeResources();

        seSceneObject *obj = nullptr;

        obj = new spriteEngine::seSprite(false, seResourceManager::GetInstance()->GetTexture("house.png"));
        AddItem("house", obj);
        obj->SetSize(0, 0, 0, 800, 600);
    }

    seScene::~seScene() {
        seResourceManager::Destroy();
    }

    void seScene::InitializeResources() {
        // Load shaders
        spriteEngine::seResourceManager *manager = spriteEngine::seResourceManager::GetInstance();
        manager->AddShader("campfire.vs");
        manager->AddShader("light.vs");
        manager->AddShader("light.fs");
        manager->AddShader("mountain.fs");
        manager->AddShader("sun.fs");

        // Load textures
        manager->AddTexture("house.png");
        manager->AddTexture("snowflake.png");
    }

    void seScene::Render() {
        for (auto &it : Items()) {
            it.second->Render();
        }
    }
}